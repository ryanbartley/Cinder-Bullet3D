if( NOT TARGET Cinder-Bullet3D )
	
	get_filename_component( BULLET3D_SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../src/BulletPhysics" ABSOLUTE )
	get_filename_component( CINDER_BULLET3D_INCLUDE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../src" ABSOLUTE )
	get_filename_component( CINDER_BULLET3D_SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../src/Cinder-Bullet3D" ABSOLUTE )
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
	
	if( NOT TARGET cinder )
		include( "${CINDER_PATH}/proj/cmake/configure.cmake" )
		find_package( cinder REQUIRED PATHS
			"${CINDER_PATH}/${CINDER_LIB_DIRECTORY}"
			"$ENV{CINDER_PATH}/${CINDER_LIB_DIRECTORY}" )
	endif()
		
	string( TOLOWER "${CINDER_TARGET}" CINDER_TARGET_LOWER )
	
	get_filename_component( BULLET3D_LIBS "${CMAKE_CURRENT_LIST_DIR}/../../lib/${CINDER_TARGET_LOWER}/${CMAKE_BUILD_TYPE}" ABSOLUTE )
	target_link_libraries( 	Cinder-Bullet3D PRIVATE cinder
				PUBLIC ${BULLET3D_LIBS}/libBulletSoftBody.a
				PUBLIC ${BULLET3D_LIBS}/libBulletInverseDynamicsUtils.a
				PUBLIC ${BULLET3D_LIBS}/libBulletInverseDynamics.a
				PUBLIC ${BULLET3D_LIBS}/libBulletDynamics.a
				PUBLIC ${BULLET3D_LIBS}/libBulletCollision.a
				PUBLIC ${BULLET3D_LIBS}/libLinearMath.a
				PUBLIC ${BULLET3D_LIBS}/libHACD.a
				PUBLIC ${BULLET3D_LIBS}/libBussIK.a
				PUBLIC ${BULLET3D_LIBS}/libConvexDecomposition.a
				PUBLIC ${BULLET3D_LIBS}/libBulletFileLoader.a
				PUBLIC ${BULLET3D_LIBS}/libBulletWorldImporter.a
			)


endif()
