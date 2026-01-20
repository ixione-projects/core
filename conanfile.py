from typing import Any
from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy, get, rmdir
import os

required_conan_version = ">=2.24"

class CoreConan(ConanFile):
    name = "ccore"

    description = "Core C/C++ Library"
    license = "MIT"
    author = "Treyvon Whitaker <ixione.dev@gmail.com>"
    topics = ()
    homepage = "https://github.com/ixione-projects/core/ccore#readme"
    url = ""

    tool_requires = ("cmake/[>=3.28]")

    package_type = "library"
    settings = ("arch", "build_type", "compiler", "os")
    options = {
        "shared": [True, False], 
        "fPIC": [True, False]
    }
    default_options = {
        "shared": False, 
        "fPIC": True
    }

    generators = "CMakeToolchain"

    def configure(self):
        opts: Any = self.options
        if opts.get_safe("shared"):
            opts.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def validate(self):
        check_min_cppstd(self, 20)

    def source(self):
        if not os.path.exists(os.path.join(str(self.source_folder), "CMakeLists.txt")):
            url = "https://github.com/ixione-projects/core/archive/refs/heads/master.zip"
            self.output.info(f"Downloading source: {url}")
            get(self, url=url, strip_root=True)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE", self.source_folder, os.path.join(str(self.package_folder), "licenses"))
        cmake = CMake(self)
        cmake.install()
        rmdir(self, os.path.join(str(self.package_folder), "lib", "cmake"))
        rmdir(self, os.path.join(str(self.package_folder), "lib", "pkgconfig"))

    def package_info(self):
        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.set_property("cmake_file_name", "CCore")

        self.cpp_info.set_property("cmake_target_name", "CCore::ccore")
        self.cpp_info.set_property("pkg_config_name", "ccore")
        self.cpp_info.libs = ["ccore"]
