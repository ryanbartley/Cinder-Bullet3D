Working platforms support iOS, and MacOSX for glNext branch of Cinder, found [here](https://github.com/cinder/Cinder/tree/glNext). 

Warning:
Master Branch using new glNext branch of Cinder. In other words, you won't be able to use it with Cinder 0.8.6 and below. To use with those versions, checkout preGlNext branch. Also, samples haven't been tested on Visual Studio. 

Multiple samples in the samples folder (SoftBodyDemo, Constraints and SphereGravity demos are working but not commented and experimental). 

The implicit order of samples is as follows...

1. HelloWorld
	- This is simply the HelloWorld bullet tutorial running in cinder. This is raw Bullet and shows a lot of what is going on under the hood.
2. PhysicsPrimitives
	- This shows creating different primitives and adding them to your context and world.
![PhysicsPrimitives](https://lh4.googleusercontent.com/vD8NjzIsitawGHe6ikWLnTvFvOYLY7-oBRYTkCmYWCWr9DUu_Z08xmGOKXcBt_-YAU6_Tn3eisE=w1570-h885)
3. VisualPhysics
	- This shows wrapping a Visual (gl) object with a Physics (bullet) object, which isn't necessarily intuitive. Hopefully, this will help you out.
![VisualPhysics](https://lh4.googleusercontent.com/G_wgBt6BgKpaDHk3-t6jXd60sVJOBPPlPuqFzzKpELSN33JZOX3mvcs_ddzUzs7QD1H3FTlgDfg=w1570-h885)
4. HeightfieldTerrain
	- This shows creating a heightfield and the helpers for gl that are associated with them.
![HeightfieldTerrain](https://lh4.googleusercontent.com/EQddfgY3poqMprRIupM3QvZSK92r2hx4rdnS8BI4hhjv_R46OH9KYc_qwOmqBFETYlSLcY9k8HI=w1570-h885)
5. Constraints (when finished)
	- This will be where I represent using constraints with Cinder-Bullet3D. 
	- The sample will be modeled from the Bullet Constraint demo. 
	- Still working out how to use Constraints within my system because I don't want to wrap all of them. :)
6. SphereGravity (almost finished)
	- Contributed by [Robert Hodgin](http://roberthodgin.com/). Thanks Robert!
	- This is an advanced case, showing many optimizations using Bullet and gl. 
	- Uses instancing, UniformScaleShape ( to use the same shape with different uniform scales )
![SphereGravity](https://lh6.googleusercontent.com/rZOklk0V8gf9KN3WNWyDUQHits3wQF6MkeLyGJR2Tj0v5FVG9aVk4aAEeLRh31ee_w_aD23msP0=w1570-h885)
7. SoftBodyDemo (when finished)
	- This will model the Bullet SoftBodyDemo.
![SoftBodyDemo](https://lh3.googleusercontent.com/pOpAVjBHQ586OANkii4LoyUwNk3uC2sa7Xrvr4PPbbOtELsodqCzEHken99o0cj7ose5LjU0VFY=w1570-h885) 

You can find out more about bullet at http://bulletphysics.org/wordpress/
