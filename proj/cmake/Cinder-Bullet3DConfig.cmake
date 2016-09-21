if( NOT TARGET Cinder-Bullet3D )
	
	get_filename_component( BULLET3D_SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../src/BulletPhysics" ABSOLUTE )
	get_filename_component( CINDER_BULLET3D_INCLUDE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../src" ABSOLUTE )
	get_filename_component( CINDER_BULLET3D_SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../src/Cinder-Bullet3D" ABSOLUTE )
	get_filename_component( BULLET3D_LIBS "${CMAKE_CURRENT_LIST_DIR}/../../lib/linux" ABSOLUTE )
	get_filename_component( CINDER_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../.." ABSOLUTE )
	
	list( APPEND CINDER_BULLET3D_SOURCES
		${CINDER_BULLET3D_SOURCE_PATH}/BulletContext.cpp
		${CINDER_BULLET3D_SOURCE_PATH}/Common.cpp
		${CINDER_BULLET3D_SOURCE_PATH}/PhysicsDebugRenderable.cpp
		${CINDER_BULLET3D_SOURCE_PATH}/RigidBody.cpp
		${CINDER_BULLET3D_SOURCE_PATH}/SoftBody.cpp
	)
	
	add_library( Cinder-Bullet3D ${CINDER_BULLET3D_SOURCES} )	

	target_include_directories( Cinder-Bullet3D PUBLIC ${BULLET3D_SOURCE_PATH} ${CINDER_BULLET3D_INCLUDE_PATH} )
	target_include_directories( Cinder-Bullet3D PRIVATE BEFORE "${CINDER_PATH}/include" )
	
	target_compile_options( Cinder-Bullet3D PUBLIC "-std=c++11" )

	if( NOT TARGET cinder )
		include( "${CINDER_PATH}/proj/cmake/configure.cmake" )
		find_package( cinder REQUIRED PATHS
			"${CINDER_PATH}/${CINDER_LIB_DIRECTORY}"
			"$ENV{CINDER_PATH}/${CINDER_LIB_DIRECTORY}" )
	endif()

	target_link_libraries( 	Cinder-Bullet3D PRIVATE cinder
				PUBLIC ${BULLET3D_LIBS}/libBulletSoftBody_gmake_x64_release.a
				PUBLIC ${BULLET3D_LIBS}/libBulletInverseDynamicsUtils_gmake_x64_release.a
				PUBLIC ${BULLET3D_LIBS}/libBulletInverseDynamics_gmake_x64_release.a
				PUBLIC ${BULLET3D_LIBS}/libBulletDynamics_gmake_x64_release.a
				PUBLIC ${BULLET3D_LIBS}/libBulletCollision_gmake_x64_release.a
				PUBLIC ${BULLET3D_LIBS}/libLinearMath_gmake_x64_release.a
				PUBLIC ${BULLET3D_LIBS}/libHACD_gmake_x64_release.a
				PUBLIC ${BULLET3D_LIBS}/libBussIK_gmake_x64_release.a
				PUBLIC ${BULLET3D_LIBS}/libConvexDecomposition_gmake_x64_release.a
				PUBLIC ${BULLET3D_LIBS}/libBulletFileLoader_gmake_x64_release.a
				PUBLIC ${BULLET3D_LIBS}/libBulletWorldImporter_gmake_x64_release.a
			)


endif()
