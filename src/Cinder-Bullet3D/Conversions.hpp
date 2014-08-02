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
inline btQuaternion toBullet( const ci::Quatf & cinderQuat ) {
	btQuaternion bulletQuat( cinderQuat.v.x, cinderQuat.v.y, cinderQuat.v.z, cinderQuat.w );
	return bulletQuat;
}
inline ci::Quatf fromBullet( const btQuaternion & bulletQuaternion ) {
	ci::Quatf cinderQuat( bulletQuaternion.w(), bulletQuaternion.x(), bulletQuaternion.y(), bulletQuaternion.z() );
	return cinderQuat;
}

inline btVector3 toBullet( const ci::Vec3f & cinderVec ) {
	btVector3 bulletVec( cinderVec.x, cinderVec.y, cinderVec.z );
	return bulletVec;
}
inline ci::Vec3f fromBullet( const btVector3 & bulletVec ) {
	ci::Vec3f cinderVec( bulletVec.x(), bulletVec.y(), bulletVec.z() );
	return cinderVec;
}

inline ci::Matrix44f fromBullet( const btTransform &bulletTrans )
{
	ci::Matrix44f transform;
	ATTRIBUTE_ALIGNED16(btScalar m[16]);
	bulletTrans.getOpenGLMatrix( m );
	transform.set(m);
	return m;
}
inline btTransform toBullet( const ci::Matrix44f &cinderMat4 )
{
	btTransform trans;
	trans.setFromOpenGLMatrix( cinderMat4.m );
	return trans;
}
	
inline ci::Matrix33f fromBullet( const btMatrix3x3 &bulletMat3 )
{
	ci::Matrix33f m;
	bulletMat3.getOpenGLSubMatrix( m.m );
	return m;
}
	
inline btMatrix3x3 toBullet( const ci::Matrix33f &cinderMat3 )
{
	btMatrix3x3 trans;
	trans.setFromOpenGLSubMatrix( cinderMat3.m );
	return trans;
}
	
}