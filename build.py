"""
This file is part of the Woffle C compiler
See LICENSE for license
See README.md for more info about the project
"""

import os
import sys
import time
import colorama
import subprocess

SRC_DIR = "src"
BUILD_DIR = "build"
INCLUDE_DIR = "include"
BIN_DIR = "bin"

class CompilationHelper:
    def __init__(self):
        self.ldflags = []
        self.cflags = ["-std=c99", f"-I{INCLUDE_DIR}"]

        self.cc = "clang"
        self.target = sys.argv[1] if len(sys.argv) > 1 and sys.argv[1] in ("debug", "release") else "release"
        if self.target == "debug":
            self.cflags += ["-fsanitize=address,undefined", "-Wall", "-Werror", "-Wextra", "-g3", "-ggdb", "-O0"]
        if len(sys.argv) > 1 and sys.argv[1] == "clean":
            self.command(f"rm -rf {BUILD_DIR} {BIN_DIR}")
            exit(0)

    def info(self, text):
        print(f"({time.strftime('%X')})   {colorama.Fore.CYAN}[INFO] {text}{colorama.Style.RESET_ALL}")

    def warning(self, text):
        print(f"({time.strftime('%X')})   {colorama.Fore.YELLOW}[WARN] {text}{colorama.Style.RESET_ALL}")

    def error(self, text):
        print(f"({time.strftime('%X')})   {colorama.Fore.RED}[ERR] {text}{colorama.Style.RESET_ALL}")

    def command(self, command):
        print(f"({time.strftime('%X')})   {colorama.Fore.CYAN}[CMD] {command}{colorama.Style.RESET_ALL}")
        proc = subprocess.run(command.split(" "), stderr=subprocess.PIPE)
        if proc.returncode != 0:
            print(proc.stderr.decode("UTF-8"))
            exit(1)


    def ensure_dir(self, directory):
        if not os.path.exists(directory):
            self.info(f"Creating directory {directory}")
            os.mkdir(directory)

    def ensure_dirs(self):
        self.ensure_dir(BUILD_DIR)
        self.ensure_dir(BIN_DIR)
        self.ensure_dir(f"{BIN_DIR}/{self.target}")

    def resolve_environ(self):
        if "CC" in os.environ:
            try:
                self.info(f"Testing C compiler '{os.environ['CC']}'")
                proc = subprocess.run([os.environ["CC"], "-v"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
                self.cc = os.environ["CC"]
            except Exception:
                self.error(f"Invalid C compiler ('{os.environ['CC']}')")
        if "LDFLAGS" in os.environ:
            self.ldflags += os.environ["LDFLAGS"].split(",")
        if "CFLAGS" in os.environ:
            self.cflags += os.environ["CFLAGS"].split(",")

    def compile_file(self, file):
        self.command(f"{self.cc} {' '.join(self.cflags)} -c -o {BUILD_DIR}/{file.split('/')[-1].replace('.c', '.o')} {file}")

    def link_files(self, directory):
        files = os.listdir(directory)
        self.command(f"{self.cc} {' '.join(self.cflags)} -o {BIN_DIR}/{self.target}/wcc {' '.join(directory + '/' + file for file in files)} {' '.join(self.ldflags)}")

    def compile_dir(self, directory):
        for file in os.listdir(directory):
            if file.endswith(".c"):
                self.compile_file(f"{directory}/{file}")
            elif os.path.isdir(f"{directory}/{file}"):
                self.compile_dir(f"{directory}/{file}")

    def compile_all(self):
        self.ensure_dirs()
        self.resolve_environ()
        self.compile_dir(SRC_DIR)
        self.link_files(BUILD_DIR)

if __name__ == "__main__":
    helper = CompilationHelper()
    helper.compile_all()

