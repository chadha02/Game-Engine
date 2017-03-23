Actor =
{
	name = "Monster",
	class= "BadGuy",
	initial_position = {200.0, -80.0, 0.0},
	initial_velocity = {0.0, 0.0, 0.0},
	initialRotation = 0.0,
	
	physics_settings = {
			mass = 5.0,

	},
	
	bounding_box = { 
			center	= { 0.0, 118.0, 0.0 },
			extents	= { 160.0, 108.0, 0.0 }
	},
	
	render_settings = {
			spriteFile = "data\\BadGuy.dds",
	}


}