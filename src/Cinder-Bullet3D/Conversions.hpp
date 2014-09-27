//
//  Conversions.hpp
//  Prototype_2
//
//  Created by Ryan Bartley on 2/13/14.
//
//

#pragma once

namespace bullet {
	
// Beginning Conversion from type to type
inline btQuaternion toBullet( const ci::quat & cinderQuat ) {
	btQuaternion bulletQuat( cinderQuat.x, cinderQuat.y, cinderQuat.z, cinderQuat.w );
	return bulletQuat;
}
inline ci::quat fromBullet( const btQuaternion & bulletQuaternion ) {
	ci::quat cinderQuat( bulletQuaternion.w(), bulletQuaternion.x(), bulletQuaternion.y(), bulletQuaternion.z() );
	return cinderQuat;
}

inline btVector3 toBullet( const ci::vec3 & cinderVec ) {
	btVector3 bulletVec( cinderVec.x, cinderVec.y, cinderVec.z );
	return bulletVec;
}
inline ci::vec3 fromBullet( const btVector3 & bulletVec ) {
	ci::vec3 cinderVec( bulletVec.x(), bulletVec.y(), bulletVec.z() );
	return cinderVec;
}

inline ci::mat4 fromBullet( const btTransform &bulletTrans )
{
	ATTRIBUTE_ALIGNED16(btScalar m[16]);
	bulletTrans.getOpenGLMatrix( m );
	return glm::make_mat4( m );
}
inline btTransform toBullet( const ci::mat4 &cinderMat4 )
{
	btTransform trans;
	trans.setFromOpenGLMatrix( &cinderMat4[0][0] );
	return trans;
}
	
inline ci::mat3 fromBullet( const btMatrix3x3 &bulletMat3 )
{
	ATTRIBUTE_ALIGNED16(btScalar m[12]);
	bulletMat3.getOpenGLSubMatrix( m );
	return glm::make_mat3( m );
}
	
inline btMatrix3x3 toBullet( const ci::mat3 &cinderMat3 )
{
	btMatrix3x3 trans;
	trans.setFromOpenGLSubMatrix( &cinderMat3[0][0] );
	return trans;
}
	
}