import os
import subprocess
import logging
import re
from rich import print as p


def testYASSFiles():
    tests = [
                "conditionals.yass",
                "identifier.yass",
                "iterative.yass",
                "opertators.yass"
            ]

    for file in tests:
        filepath = f"./test/{file}"
        subproc = subprocess.Popen([".//yassou", filepath, "-d"],stdout=subprocess.PIPE)
        if (subproc.returncode == 0):
            p(f"[b green]Test successful")
        else:
            for i in subproc.communicate():
                print(f"{i}")
            p(f"[b red]Return code: {subproc.returncode}. Test failed on {file}. Error: {subproc.stderr}.")
            return;

testYASSFiles()