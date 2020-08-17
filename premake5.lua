workspace 'CppPlayground'
	architecture 'x86_64'

	configurations
	{
		'Debug',
		'Release',
	}

	flags
	{
		'MultiProcessorCompile',
	}

outputdir = '%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}'

function make_proj(src_dir, proj_kind, name)

	global_name = name or src_dir
	project(global_name)
	location(src_dir)
	warnings 'Extra'
	language 'C++'
	cppdialect 'C++17'
	staticruntime 'off'
	toolset 'clang'

	local is_test = false
	if (proj_kind == 'Test') then
		is_test = true
		proj_kind = 'ConsoleApp'
	end

	kind(proj_kind or 'SharedLib')

	targetdir(_MAIN_SCRIPT_DIR .. '/bin/' .. outputdir)
	objdir(_MAIN_SCRIPT_DIR .. '/bin-int/' .. outputdir .. '/%{prj.name}')

	files
	{
		src_dir .. '/*.h',
		src_dir .. '/*.cpp',
	}

	includedirs
	{
		_MAIN_SCRIPT_DIR .. '/Libraries',
	}

	sysincludedirs
	{
		_MAIN_SCRIPT_DIR .. '/third_party/v8/include',
		_MAIN_SCRIPT_DIR .. '/third_party/glm',
		_MAIN_SCRIPT_DIR .. '/third_party/GLFW/include',
		_MAIN_SCRIPT_DIR .. '/third_party/glad/include',
		_MAIN_SCRIPT_DIR .. '/third_party/spdlog/include',
		_MAIN_SCRIPT_DIR .. '/third_party/imgui',
		_MAIN_SCRIPT_DIR .. '/third_party/stb',
	}

	if (is_test) then
		files
		{
			_MAIN_SCRIPT_DIR .. '/third_party/catch2/include/catch2/catch2.hpp',
			_MAIN_SCRIPT_DIR .. '/third_party/catch2/include/catch2/catch2.cpp',
		}

		sysincludedirs
		{
			_MAIN_SCRIPT_DIR .. '/third_party/catch2/include',
		}
	end

	filter { 'kind:SharedLib', 'system:windows' }
		defines 'COMPILING_DLL'

	filter { 'system:linux', 'configurations:Debug' }
		linkoptions { '-rdynamic' }

	filter 'configurations:Debug'
		defines 'DEBUG'
		runtime 'Debug'
		symbols 'on'

	filter 'configurations:Release'
		defines 'RELEASE'
		runtime 'Release'
		optimize 'on'

	-- Reset filter
	filter {}
end

function useSTB(table)
	for key, value in pairs(table) do
		files
		{
			_MAIN_SCRIPT_DIR .. '/third_party/stb/stb_' .. value .. '.h',
			_MAIN_SCRIPT_DIR .. '/third_party/stb/stb_' .. value .. '.cpp',
		}
	end
end

function withTest()
	local proj_name = global_name
	make_proj(proj_name .. '/Tests', 'Test', 'Test' .. proj_name)
		links { proj_name }
end

group 'Libraries'
	include './Libraries'

group 'Applications'
	include './Applications'

group 'Game'
	include './Game'

group 'Dependencies'
	include './third_party/GLFW'
	include './third_party/glad'
	include './third_party/imgui'

group ''
