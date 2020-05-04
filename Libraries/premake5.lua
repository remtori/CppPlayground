make_proj("TestASL", "./ASL/Tests", "Test")

make_proj("ASL", "./ASL")
	excludes
	{
		"./ASL/Tests/**.h",
		"./ASL/Tests/**.cpp"
	}
