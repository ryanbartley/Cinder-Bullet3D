Working platforms support iOS, MacOSX, and Windows on master and android\_linux branches and linux on the android\_linux branch of Cinder, found [here](https://github.com/cinder/Cinder/tree/glNext). 

Installation
============
* First run `git submodule update --init` in the repo's root.
* For Windows, execute the install.bat in install subdirectory. `cd install && install.bat` (then get some coffee)
* For OSX, IOS, and Linux, execute the install.sh in install subdirectory, giving the platform you wish to build as an argument, i.e. `cd install && ./install.sh macosx` (also, go ahead and get some fresh air).

The above will build the binaries of Bullet for you and place them where they need to go. After they're done, have at it!

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
	- This shows creating a heightfield and the helpers for gl that are associated with them. Right now there's a disconnect between the visual object of 
![HeightfieldTerrain](https://cloud.githubusercontent.com/assets/2651863/4435557/83dbbd16-474a-11e4-8171-a41c3bdd2084.png)
5. RayCasting
	- Shows the operation of raycasting into the bullet context (the world), selecting an object and the ability to manipulate that object in bullet or in your own program. It also shows the use of a simple constraint.
6. TriggerVolume
	- Shows the operation of creating a trigger and adding that to the world. Triggers are great for explosions, or knowing when something passes an obstacle, etc. It doesn't cause a physical effect in the world but it will issue a collision to your collision listener. You can use this with logic to "trigger" effects.
7. RagDoll 
	- Shows the constraints you'd use to create a ragdoll. (Converted from Bullet Sample)
![RagDoll](https://cloud.githubusercontent.com/assets/2651863/4435556/83dabd80-474a-11e4-95e3-050a7b174048.png)
8. Constraints (when finished)
	- This will be where I represent using constraints with Cinder-Bullet3D. 
	- The sample will be modeled from the Bullet Constraint demo. 
	- Still working out how to use Constraints within my system because I don't want to wrap all of them. :)
9. SphereGravity (almost finished)
	- Contributed by [Robert Hodgin](http://roberthodgin.com/). Thanks Robert!
	- This is an advanced case, showing many optimizations using Bullet and gl. 
	- Uses instancing, UniformScaleShape ( to use the same shape with different uniform scales )
![SphereGravity](https://cloud.githubusercontent.com/assets/2651863/4435555/83da7578-474a-11e4-838c-90e4ca4d366f.png)
10. SoftBodyDemo (when finished)
	- This will model the Bullet SoftBodyDemo.
	- Note: assertion happening on debug. Run in Release at your own risk! 
![SoftBodyDemo](https://cloud.githubusercontent.com/assets/2651863/4435554/83d6cb8a-474a-11e4-8849-2c329edbae37.png) 

You can find out more about bullet at http://bulletphysics.org/wordpress/
