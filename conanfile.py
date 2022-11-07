from conans import ConanFile, CMake

class FalloutDiceRollCppConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    # requires =
    generators = "cmake_paths"
    default_options = {}
    
    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib", dst="bin", src="lib")
