#!/usr/bin/env python3
import argparse
import os

def read_file(path):
    with open(path, "r", encoding="utf-8") as f:
        return f.readlines()

def check_boilerplate(src_dir, boilerplate_path, exts=None):
    boilerplate_lines = read_file(boilerplate_path)
    boilerplate_stripped = [line.strip() for line in boilerplate_lines]

    results = []
    for root, _, files in os.walk(src_dir):
        for file in files:
            if exts and not any(file.endswith(ext) for ext in exts):
                continue
            file_path = os.path.join(root, file)
            try:
                file_lines = read_file(file_path)
                file_head = [line.strip() for line in file_lines[:len(boilerplate_lines)]]

                if file_head == boilerplate_stripped:
                    results.append((file_path, True))
                else:
                    results.append((file_path, False))
            except Exception as e:
                results.append((file_path, f"Error: {e}"))
    return results

def main():
    parser = argparse.ArgumentParser(description="Check boilerplate compliance in source files.")
    parser.add_argument("--src", required=True, help="Path to source directory")
    parser.add_argument("--boilerplate", required=True, help="Path to boilerplate template file")
    parser.add_argument("--ext", nargs="*", default=[".cpp", ".h", ".py"], 
                        help="File extensions to check (default: .cpp .h .py)")
    args = parser.parse_args()

    results = check_boilerplate(args.src, args.boilerplate, args.ext)

    print("\nBoilerplate check results:")
    for path, status in results:
        if status is True:
            print(f"[OK]    {path}")
        elif status is False:
            print(f"[FAIL]  {path}")
        else:
            print(f"[ERR]   {path} -> {status}")

    failed = sum(1 for _, s in results if s is False)
    passed = sum(1 for _, s in results if s is True)
    print(f"\nSummary: {passed} passed, {failed} failed, {len(results)} total files")

if __name__ == "__main__":
    main()
