Actor =
{
	name = "Player1",
	class= "GoodGuy",
	initial_position = {-200.0, -80.0, 0.0},
	initial_velocity = {1.0, 0.0, 0.0},
	initialRotation = 0.0,
	
	physics_settings = {
			mass = 1.0,

	},
	bounding_box = { 
			center	= { 0.0, 82.0, 0.0 },
			extents	= { 44.0, 78.0, 0.0 }
	},
	
	render_settings = {
			spriteFile = "data\\GoodGuy.dds",	
	}


}