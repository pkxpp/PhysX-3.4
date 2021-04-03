//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2018 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#ifdef RENDER_SNIPPET

#include <vector>

#include "PxPhysicsAPI.h"

#include "../SnippetRender/SnippetRender.h"
#include "../SnippetRender/SnippetCamera.h"
#include "wavefront.h"

using namespace physx;

extern void initPhysics(bool interactive);
extern void stepPhysics(bool interactive);	
extern void cleanupPhysics(bool interactive);
extern void keyPress(unsigned char key, const PxTransform& camera);

static std::vector<char*>		gSearchPaths;
static PxTriangleMesh*			mesh = NULL;
extern PxPhysics*				gPhysics;
extern PxCooking*				gCooking;

namespace
{
Snippets::Camera*	sCamera;

void motionCallback(int x, int y)
{
	sCamera->handleMotion(x, y);
}

void keyboardCallback(unsigned char key, int x, int y)
{
	if(key==27)
		exit(0);

	if(!sCamera->handleKey(key, x, y))
		keyPress(key, sCamera->getTransform());
}

void mouseCallback(int button, int state, int x, int y)
{
	sCamera->handleMouse(button, state, x, y);
}

size_t strlcpy(char* dst, size_t dstSize, const char* src)
{
	size_t i = 0;
	if (dst && dstSize)
	{
		for (; i + 1 < dstSize && src[i]; i++) // copy up to dstSize-1 bytes
			dst[i] = src[i];
		dst[i] = 0; // always null-terminate
	}

	while (src[i]) // read any remaining characters in the src string to get the length
		i++;

	return i;
}

size_t strlcat(char* dst, size_t dstSize, const char* src)
{
	size_t i = 0, s = 0;
	if (dst && dstSize)
	{
		s = strlen(dst);
		for (; i + s + 1 < dstSize && src[i]; i++) // copy until total is at most dstSize-1
			dst[i + s] = src[i];
		dst[i + s] = 0; // always null-terminate
	}

	while (src[i]) // read any remaining characters in the src string to get the length
		i++;

	return i + s;
}

const char* findPath(const char* path)
{
	static char fullPath[512];

	::FILE* file = NULL;
	const PxU32 numSearchPaths = (PxU32)gSearchPaths.size();
	for (PxU32 i = 0; i < numSearchPaths; i++)
	{
		const char* prefix = gSearchPaths[i];
		strlcpy(fullPath, 512, prefix);
		strlcat(fullPath, 512, path);

		//for (unsigned i = 0; i < strlen(fullPath); ++i) 
		//{
		//	if (fullPath[i] == '\\' || fullPath[i] == '/')
		//		fullPath[i] = '\\';
		//}

		errno_t err = ::fopen_s(&file, fullPath, "rb");
		if (file) break;
	}
	if (file)
	{
		fclose(file);
		return fullPath;
	}
	else
		return path;
}

void idleCallback()
{
	glutPostRedisplay();
}

void renderCallback()
{
	stepPhysics(true);

	Snippets::startRender(sCamera->getEye(), sCamera->getDir());

	PxScene* scene;
	PxGetPhysics().getScenes(&scene,1);
	PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	if(nbActors)
	{
		std::vector<PxRigidActor*> actors(nbActors);
		scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
		Snippets::renderActors(&actors[0], static_cast<PxU32>(actors.size()), true);
	}

	// test
	if (mesh)
	{
		
		const physx::PxVec3 color = physx::PxVec3(1.0f, 0.f, 0.0f);
		const PxMat44 pos(PxTransform(PxVec3(0.0f, 5.0f, 0.0f)));
		PxGeometryHolder h;

		/*if (shapes[j]->getFlags() & PxShapeFlag::eTRIGGER_SHAPE)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
		PxTriangleMeshGeometry geo;
		geo.triangleMesh = mesh;
		geo.scale = PxMeshScale(10.0f);
		h.storeAny(geo);

		// render object
		glPushMatrix();
		glMultMatrixf(reinterpret_cast<const float*>(&pos));
		
		glColor4f(color.x, color.y, color.z, 1.0f);

		renderGeometry(h);
		glPopMatrix();
	}
	// test end

	Snippets::finishRender();
}

void exitCallback(void)
{
	delete sCamera;
	cleanupPhysics(true);
}
}

const char* getSampleOutputFilePath(const char* inFilePath, const char* outExtension)
{
	static char sBuffer[1024];

	sprintf(sBuffer, "..\\..\\..\\Media\\PhysX\\3.4\\Samples\\cached\\%s%s", inFilePath, outExtension);
	return sBuffer;
}

void testCreateObj()
{
	// test
	gSearchPaths.push_back("..\\..\\..\\Media\\PhysX\\3.4\\Samples\\");
	//gSearchPaths.push_back("../../media/");
	//gSearchPaths.push_back(".. / .. / media / SampleRenderer / 4 /");
	{
		const char* filename = findPath("motor_body_Reduced.obj");
		WavefrontObj wfo;
		if (!wfo.loadObj(filename, false))
		{
			printf("Error loading file: %s\n", filename);
		}

		//PxDefaultFileInputData cooked(filename);
		if (!mesh)
		{
			const char* filenameCooked = getSampleOutputFilePath("tree.obj", "");
			PxTriangleMeshDesc meshDesc;
			meshDesc.points.count = wfo.mVertexCount;
			meshDesc.triangles.count = wfo.mTriCount;
			meshDesc.points.stride = sizeof(float) * 3;
			meshDesc.triangles.stride = sizeof(int) * 3;
			meshDesc.points.data = wfo.mVertices;
			meshDesc.triangles.data = wfo.mIndices;

			bool ok;
			{
				PxDefaultFileOutputStream stream(filenameCooked);
				ok = gCooking->cookTriangleMesh(meshDesc, stream);
			}
			if (ok)
			{
				PxDefaultFileInputData stream(filenameCooked);
				mesh = gPhysics->createTriangleMesh(stream);
			}
		}
	}

	// test end
}

void renderLoop()
{

	sCamera = new Snippets::Camera(PxVec3(50.0f, 50.0f, 50.0f), PxVec3(-0.6f,-0.2f,-0.7f));

	Snippets::setupDefaultWindow("PhysX Snippet HelloWorld");
	Snippets::setupDefaultRenderState();

	glutIdleFunc(idleCallback);
	glutDisplayFunc(renderCallback);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);
	motionCallback(0,0);

	atexit(exitCallback);

	initPhysics(true);
	testCreateObj();

	glutMainLoop();
}
#endif
