Warning: Master Branch using new glNext branch of Cinder. In other words, you won't be able to use it with Cinder 0.8.6 and below. To use with those versions, checkout preGlNext branch. Also, samples haven't been tested on Visual Studio. 

Working platforms support iOS, and MacOSX for glNext branch of Cinder, found [here](https://github.com/cinder/Cinder/tree/glNext). 

Multiple samples in the samples folder (SoftBodyDemo, Constraints and SphereGravity demos are working but not commented and experimental). 

The implicit order of samples is as follows...

1. HelloWorld
	- This is simply the HelloWorld bullet tutorial running in cinder. This is raw Bullet and shows a lot of what is going on under the hood.
2. PhysicsPrimitives
	- This shows creating different primitives and adding them to your context and world.
	![PhysicsPrimitives](https://drive.google.com/file/d/0B_9cvCpvdnyHUDM5S0R3ckNWMVU/edit?usp=sharing)
3. VisualPhysics
	- This shows wrapping a Visual (gl) object with a Physics (bullet) object, which is surprisingly complicated. Hopefully, this will help you out.
4. HeightfieldTerrain
	- This shows creating a heightfield and the helpers for gl that are associated with them.
5. Constraints (when finished)
	- This will be where I represent using constraints with Cinder-Bullet3D. 
	- The sample will be modeled from the Bullet Constraint demo. 
	- Still working out how to use Constraints within my system because I don't want to wrap all of them. :)
6. SphereGravity (almost finished)
	- Contributed by [Robert Hodgin](http://roberthodgin.com/). Thanks Robert!
	- This is an advanced case, showing many optimizations using Bullet and gl. 
	- Uses instancing, UniformScaleShape ( to use the same shape with different uniform scales )
7. SoftBodyDemo (when finished)
	- This will model the Bullet SoftBodyDemo.

You can find out more about bullet at http://bulletphysics.org/wordpress/
