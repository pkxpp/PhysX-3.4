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
// Copyright (c) 2008-2019 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

// ****************************************************************************
// This snippet illustrates simple use of physx
//
// It creates a number of box stacks on a plane, and if rendering, allows the
// user to create new stacks and fire a ball from the camera position
// ****************************************************************************

#include <ctype.h>

#include "PxPhysicsAPI.h"

#include "../SnippetCommon/SnippetPrint.h"
#include "../SnippetCommon/SnippetPVD.h"
#include "../SnippetUtils/SnippetUtils.h"
#include "geometry/PxGeometryQuery.h"
#include "../PhysXCallback.h"
#include <vector>


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene		= NULL;

PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

// test
PxRigidDynamic*			gPlane		= NULL;
PxCooking*				gCooking = NULL;
PxRigidStatic*			gStaticOut	= NULL;
PxRigidStatic*			gStaticIn	= NULL;
// test end

PxReal stackZ = 10.0f;

PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity=PxVec3(0))
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);

	// test
	//PxRaycastHit hitInfo;
	//PxU32 maxHits = 1;
	//PxVec3 unitDir = velocity;
	//PxHitFlags hitFlags = PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eDISTANCE | PxHitFlag::eUV;
	//PxSphereGeometry geom = PxSphereGeometry(10);
	////PxTransform pose(PxVec3(0.0f, 1.0f, 50.0f));
	//PxTransform pose = t;
	//pose.p += unitDir;
	//PxReal maxDist = 100.0f;
	//PxU32 hitCount = 0;
	//hitCount = PxGeometryQuery::raycast(t.p, unitDir,
	//	geom, pose,
	//	maxDist,
	//	hitFlags,
	//	maxHits, &hitInfo);

	bool status = false;
	PxVec3 origin = t.p;                 // [in] Ray origin
	PxVec3 unitDir = velocity.getNormalized();                // [in] Normalized ray direction
	unitDir = PxVec3(0.0f, 0.0f, 1.0f);
	PxReal maxDistance = 1000.0f;          // [in] Raycast max distance
	PxRaycastBuffer hit;                 // [out] Raycast results

										 // Raycast against all static & dynamic objects (no filtering)
										 // The main result from this call is the closest hit, stored in the 'hit.block' structure
	status = gScene->raycast(origin, unitDir, maxDistance, hit);

	//PxSweepBuffer hitSweep;
	//PxSphereGeometry sweepShape = PxSphereGeometry(30);
	//status = gScene->sweep(sweepShape, t, unitDir, PxReal(100.0f), hitSweep);


	PxOverlapBuffer hitOverlap;            // [out] Overlap results
	PxSphereGeometry overlapShape = PxSphereGeometry(50);
	status = gScene->overlap(overlapShape, t, hitOverlap);

	//if (status)
	//	applyDamage(hit.block.position, hit.block.normal);


	if (false)
	{
	//gScene->addActor(*dynamic);

	// test
	//PxRaycastHit hitInfo;
	//PxU32 maxHits = 1;
	//PxVec3 unitDir = velocity;
	//PxHitFlags hitFlags = PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eDISTANCE | PxHitFlag::eUV;
	//PxSphereGeometry geom = PxSphereGeometry(10);
	////PxTransform pose(PxVec3(0.0f, 1.0f, 50.0f));
	//PxTransform pose = t;
	//pose.p += unitDir;
	//PxReal maxDist = 100.0f;
	//PxU32 hitCount = 0;
	//hitCount = PxGeometryQuery::raycast(t.p, unitDir,
	//	geom, pose,
	//	maxDist,
	//	hitFlags,
	//	maxHits, &hitInfo);

	bool status = false;
	PxVec3 origin = t.p;                 // [in] Ray origin
	PxVec3 unitDir = velocity.getNormalized();                // [in] Normalized ray direction
	//unitDir = PxVec3(0.0f, 0.0f, 1.0f);
	//PxReal maxDistance = 1000.0f;          // [in] Raycast max distance
	PxRaycastBuffer hit;                 // [out] Raycast results

										 // Raycast against all static & dynamic objects (no filtering)
										 // The main result from this call is the closest hit, stored in the 'hit.block' structure
	//status = gScene->raycast(origin, unitDir, maxDistance, hit);

	PxSweepHit hitSweepTouch[64];
	PxSweepBuffer hitSweep(hitSweepTouch, 64);
	PxSphereGeometry sweepShape = PxSphereGeometry(100);
	PxHitFlags hitFlags = PxHitFlags(PxHitFlag::eDEFAULT);
	PxQueryFilterData filterData = PxQueryFilterData(PxQueryFlag::eSTATIC);
	status = gScene->sweep(sweepShape, t, unitDir, PxReal(0.0f), hitSweep, hitFlags, filterData);
	//bool bInitOverlap = hitSweep.block.hadInitialOverlap();
	//{
	//	printf("hitSweep.hasBlock = %s\n", hitSweep.hasBlock ? "true" : "false");
	//	printf("hitSweep.block.hadInitialOverlap() = %s\n", bInitOverlap ? "true" : "false");
	//}

	PxOverlapBuffer hitOverlap;            // [out] Overlap results
	PxSphereGeometry overlapShape = PxSphereGeometry(50);
	//status = gScene->overlap(overlapShape, t, hitOverlap);

	//if (status)
	//	applyDamage(hit.block.position, hit.block.normal);

	PxOverlapHit hitOverlapTouch[64];
	//PhysXOverlapCallback overlapCallback(&(hitOverlapTouch[0]), 64);
	PxOverlapBuffer overlapBuffer(&(hitOverlapTouch[0]), 64);
	if (false && gStaticIn) {
		//PxShape* shape = gPhysics->createShape(PxBoxGeometry(3, 3, 3), *gMaterial);
		//PxGeometryHolder h = shape->getGeometry();
		//PxTransform t1 = PxTransform(PxVec3(0, 10, stackZ = 10.0f));
		//status = gScene->overlap(h.any(), t1, overlapBuffer);

		// Ð¡·½¿éµ±×ögeometry
		PxTransform tIn = gStaticIn->getGlobalPose();
		int nShapes = gStaticIn->getNbShapes();
		PxShape* shapes[1];
		gStaticIn->getShapes(shapes, nShapes);
		PxGeometryHolder hIn = shapes[0]->getGeometry();
		status = gScene->overlap(hIn.any(), tIn, overlapBuffer);
	}
	

	// °üº¬¹ØÏµµÄÅö×²¼ì²â
	PxOverlapBuffer hitInOutOverlap;
	if (false && gStaticOut)
	{
		PxTransform tIn = gStaticIn->getGlobalPose();
		int nShapes = gStaticIn->getNbShapes();
		PxShape* shapes[1];
		gStaticIn->getShapes(shapes, nShapes);
		PxGeometryHolder hIn = shapes[0]->getGeometry();

		status = gScene->overlap(hIn.any(), tIn, hitInOutOverlap);

		PxTransform tOut = gStaticOut->getGlobalPose();
		nShapes = gStaticOut->getNbShapes();
		PxShape* shapesOut[1];
		gStaticOut->getShapes(shapesOut, nShapes);
		PxGeometryHolder hOut = shapesOut[0]->getGeometry();

		bool bIsOverlaping = physx::PxGeometryQuery::overlap(hIn.any(), tIn, hOut.any(), tOut);
		if (bIsOverlaping)
		{
		}
	}
	
	

	

	//status = gScene->overlap(hIn.any(), tIn, hitInOutOverlap);
	//status = gScene->overlap(hOut.any(), tOut, hitInOutOverlap);

	//PxSweepBuffer hitInSweep;
	//status = gScene->sweep(hIn.any(), tIn, unitDir, PxReal(0.0f), hitInSweep, hitFlags, filterData);

	
	// add forceµÄÒ»Ð©²âÊÔ
	if (true ) {
		PxU32 nbActors = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC);
		if (nbActors > 0)
		{
			std::vector<PxActor*> actors(nbActors);
			gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC, &actors[0], nbActors);
			PxVec3 v3Force(1000.f, 0.f, 0.f);
			for (PxU32 i = 0; i < 1/*nbActors*/; i++)
			{
				
				//actors[i]->is<PxRigidBody>()->addForce(v3Force, PxForceMode::eFORCE);				// PxVec3 v3Force(1000000.f.f, 0.f, 0.f);
				//actors[i]->is<PxRigidBody>()->addForce(v3Force, PxForceMode::eIMPULSE);			// PxVec3 v3Force(10000.f, 0.f, 0.f);
				//actors[i]->is<PxRigidBody>()->addForce(v3Force, PxForceMode::eVELOCITY_CHANGE);	// PxVec3 v3Force(100.f, 0.f, 0.f);
				actors[i]->is<PxRigidBody>()->addForce(v3Force, PxForceMode::eACCELERATION);		// PxVec3 v3Force(1000.f, 0.f, 0.f);
				printf("mass-----[%f]-[%f]\n", actors[i]->is<PxRigidBody>()->getInvMass(), actors[i]->is<PxRigidBody>()->getMass());
			}
		}
	}
	}
	// test

	return dynamic;
}

PxRigidDynamic* createPlane(const PxTransform& t, PxVec3 v)
{
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(v.x, v.y, v.z), *gMaterial);
	
	PxRigidDynamic* body = gPhysics->createRigidDynamic(t);
	body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	//body->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
	body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	body->attachShape(*shape);
	// test
	//body->setMass(655350000.f);

	//body->setMass(50000.f);
	//body->setMassSpaceInertiaTensor(PxVec3(0.f));
	// test end
	//PxRigidBodyExt::updateMassAndInertia(*body, 100.0f);
	gScene->addActor(*body);

	shape->release();
	return body;
}

void movePlane(PxRigidDynamic* body, const PxVec3 speed)
{
	PxTransform t;
	//body->getKinematicTarget(t);
	t = body->getGlobalPose();
	t.p += speed;
	body->setKinematicTarget(t);

	PxTransform t1 = body->getGlobalPose();
	PxTransform t2;
	body->getKinematicTarget(t2);
	t = body->getGlobalPose();
	//t.p += speed;
	//body->setGlobalPose(t);
}

PxRigidStatic* createStaticStack(const PxTransform& t, PxReal halfExtent)
{
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	/*for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{*/
			PxRigidStatic* body = gPhysics->createRigidStatic(t);
			body->attachShape(*shape);
			//PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			gScene->addActor(*body);
	//	}
	//}
	shape->release();

	return body;
}

void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent, PxReal density = 1.0f)
{
	PxRigidActor* actor = NULL;
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	for(PxU32 i=0; i<size;i++)
	{
		for(PxU32 j=0;j<size-i;j++)
		{
			PxTransform localTm(PxVec3(PxReal(j*2) - PxReal(size-i), PxReal(i*2+1), 0) * halfExtent);
			if (density > 0.01f)
			{
				actor = gPhysics->createRigidDynamic(t.transform(localTm));
			}
			else
			{
				actor = gPhysics->createRigidStatic(t.transform(localTm));
			}
			
			actor->attachShape(*shape);
			//body->setMass(density);
			if (density > 0.01f)
			{
				PxRigidDynamic* body = static_cast<PxRigidDynamic*>(actor);
				PxRigidBodyExt::updateMassAndInertia(*body, 1.0f);
			}
			else
			{
				//body->setMassSpaceInertiaTensor(PxVec3(0.f));
				//body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
				gPlane->setMass(20000000.0f);
				gPlane->setMassSpaceInertiaTensor(PxVec3(0.f));
			}
			
			
			
			gScene->addActor(*actor);
		}
	}
	shape->release();
}

void initPhysics(bool interactive)
{
	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);
	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(PxTolerancesScale()));

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher	= gDispatcher;
	sceneDesc.filterShader	= PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if(pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(1.0f, 1.0f, .0f);
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	//gMaterial = gPhysics->createMaterial(0.f, 0.f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	gScene->addActor(*groundPlane);

	const float fStartHeight = 30.0f;
	// test
	gPlane = createPlane(PxTransform(PxVec3(0, fStartHeight, 0.0f)), PxVec3(30.0f, 1.0f, 30.0f));
	createStack(PxTransform(PxVec3(0, 0.f, 100.f)), 1, 50.0f, 0.0f);
	// test end

	for(PxU32 i=0;i<3;i++)
		createStack(PxTransform(PxVec3(0, fStartHeight, stackZ-=10.0f)), 10, 2.0f);
/*for(PxU32 i=0;i<1;i++)
		createStack(PxTransform(PxVec3(0,0,stackZ-=10.0f)), 10, 2.0f);*/
	for (PxU32 i = 0; i < 1; i++)
		createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 1, 2.0f);

	gStaticOut = createStaticStack(PxTransform(PxVec3(-10.f, 10, stackZ = 10.0f)), 6.0f);
	//gStaticIn = createStaticStack(PxTransform(PxVec3(0, 10, stackZ = 10.0f)), 3.0f);
	
	//gStaticIn = createStaticStack(PxTransform(PxVec3(0, 13, stackZ = 12.0f)), 6.0f);
	//gStaticIn = createStaticStack(PxTransform(PxVec3(0, 10, stackZ = 20.0f)), 3.0f);

	if(!interactive)
		createDynamic(PxTransform(PxVec3(0,40,100)), PxSphereGeometry(10), PxVec3(0,-50,-100));
}

void stepPhysics(bool interactive)
{
	PX_UNUSED(interactive);
	gScene->simulate(1.0f/60.0f);
	gScene->fetchResults(true);

	//bool status = false;
	//PxVec3 origin = PxVec3(0, 10, 0);                 // [in] Ray origin
	//PxVec3 unitDir = PxVec3(0.0f, 0.0f, 1.0f);
	//PxReal maxDistance = 1000.0f;          // [in] Raycast max distance
	//PxRaycastBuffer hit;                 // [out] Raycast results

	//									 // Raycast against all static & dynamic objects (no filtering)
	//									 // The main result from this call is the closest hit, stored in the 'hit.block' structure
	//status = gScene->raycast(origin, unitDir, maxDistance, hit);
}
	
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);
	gScene->release();
	gDispatcher->release();
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
	
	printf("SnippetHelloWorld done.\n");
}

void keyPress(unsigned char key, const PxTransform& camera)
{
	switch(toupper(key))
	{
	case 'B':	createStack(PxTransform(PxVec3(0,0,stackZ-=10.0f)), 10, 2.0f);						break;
	case ' ':	createDynamic(camera, PxSphereGeometry(3.0f), camera.rotate(PxVec3(0,0,-1))*1000);	break;
	case 'M':	movePlane(gPlane, PxVec3(0, 0, 2.0f));						break;
	case 'R':	gPlane->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);						break;
	case 'C':	
	{
		float MassList[] =
		{
			6000.0f,
			6000000.0f,
			600000000.0f,
		};
		int nCount = sizeof(MassList) / sizeof(float);
		static int nIndex = 0;
		nIndex = (++nIndex) % nCount;
		gPlane->setMass(MassList[nIndex]);
		gPlane->setMassSpaceInertiaTensor(PxVec3(0.f));
	}break;
	}
}

int snippetMain(int, const char*const*)
{
#ifdef RENDER_SNIPPET
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}
