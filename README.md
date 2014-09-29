Working platforms support iOS, and MacOSX for glNext branch of Cinder, found [here](https://github.com/cinder/Cinder/tree/glNext). 

Warning:
Master Branch using new glNext branch of Cinder. In other words, you won't be able to use it with Cinder 0.8.6 and below. To use with those versions, checkout preGlNext branch. Also, samples haven't been tested on Visual Studio.

Samples
=======
(Note: SoftBodyDemo, Constraints and SphereGravity demos are working but not commented and experimental). 

The implicit order of samples is as follows...

1. HelloWorld
	- This is simply the HelloWorld bullet tutorial running in cinder. This is raw Bullet and shows a lot of what is going on under the hood.
2. PhysicsPrimitives
	- This shows creating different primitives and adding them to your context and world.
![PhysicsPrimitives](https://cloud.githubusercontent.com/assets/2651863/4435552/83cc9fb6-474a-11e4-878c-e56ee0ec2fdd.png)
3. VisualPhysics
	- This shows wrapping a Visual (gl) object with a Physics (bullet) object, which isn't necessarily intuitive. Hopefully, this will help you out.
![VisualPhysics](https://cloud.githubusercontent.com/assets/2651863/4435553/83d6824c-474a-11e4-9a86-9090dc63039a.png)
4. HeightfieldTerrain
	- This shows creating a heightfield and the helpers for gl that are associated with them.
![HeightfieldTerrain](https://cloud.githubusercontent.com/assets/2651863/4435557/83dbbd16-474a-11e4-8171-a41c3bdd2084.png)
6. RagDoll 
	- Shows the constraints you'd use to create a ragdoll. (Converted from Bullet Sample)
![RagDoll](https://cloud.githubusercontent.com/assets/2651863/4435556/83dabd80-474a-11e4-95e3-050a7b174048.png)
5. Constraints (when finished)
	- This will be where I represent using constraints with Cinder-Bullet3D. 
	- The sample will be modeled from the Bullet Constraint demo. 
	- Still working out how to use Constraints within my system because I don't want to wrap all of them. :)
6. SphereGravity (almost finished)
	- Contributed by [Robert Hodgin](http://roberthodgin.com/). Thanks Robert!
	- This is an advanced case, showing many optimizations using Bullet and gl. 
	- Uses instancing, UniformScaleShape ( to use the same shape with different uniform scales )
![SphereGravity](https://cloud.githubusercontent.com/assets/2651863/4435555/83da7578-474a-11e4-838c-90e4ca4d366f.png)
7. SoftBodyDemo (when finished)
	- This will model the Bullet SoftBodyDemo.
![SoftBodyDemo](https://cloud.githubusercontent.com/assets/2651863/4435554/83d6cb8a-474a-11e4-8849-2c329edbae37.png) 

You can find out more about bullet at http://bulletphysics.org/wordpress/
