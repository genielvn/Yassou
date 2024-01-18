import subprocess
from rich import print as p

def testYASSFiles():
    tests = [
                "conditionals.yass",
                "identifier.yass",
                "iterative.yass",
                "operators.yass"
            ]

    for file in tests:
        p(f"\n\n=== Testing {file.encode("utf-8")} ===")

        filepath = f"./test/{file}"
        subproc = subprocess.Popen([".//yassou", filepath, "-d"],stdout=subprocess.PIPE)

        for output in subproc.stdout:
            p(output.decode("utf-8").strip("\n"))

testYASSFiles()