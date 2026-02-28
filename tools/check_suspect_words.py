# This scans files in a directory for suspect words, for example foor Git merge conflict markers like "<<<"
# It can be used to check for any suspect words in code files. 

import os
import sys

# Load conflict markers from a file, markers are suspect words
def load_markers(marker_file_path):
    try:
        with open(marker_file_path, 'r', encoding='utf-8') as f:
            return [line.strip() for line in f if line.strip()]
    except Exception as e:
        print(f"Error reading marker file: {e}")
        sys.exit(2)

def find_markers_in_file(filepath, markers):
    markers = []
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as file:
        for lineno, line in enumerate(file, start=1):
            for marker in markers:
                if marker in line:
                    markers.append(lineno, marker, line.strip())
    return markers

def scan_directory_for_markers(directory, markers, file_extensions=None):
    markers_found = {}
    for root, _, files in os.walk(directory):
        for filename in files:
            if file_extensions and not filename.endswith(tuple(file_extensions)):
                continue
            filepath = os.path.join(root, filename)
            conflicts = find_markers_in_file(filepath, markers)
            if conflicts:
                markers_found[filepath] = conflicts
    return markers_found

if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Find suspect words in code files.")
    parser.add_argument("path", help="Directory or file to scan")
    parser.add_argument("--ext", nargs='*', help="Limit to specific file extensions, e.g., --ext .py .cpp .js")
    parser.add_argument("--marker-file", required=True, help="Path to file containing markers/suspect words (one per line)")

    args = parser.parse_args()

    markers = load_markers(args.marker_file)
    if not markers:
        print("No markers loaded. Please check the marker file.")
        sys.exit(2)

    detected = False

    if os.path.isfile(args.path):
        result = find_markers_in_file(args.path, markers)
        if result:
            detected = True
            print(f"\nMarkers found in: {args.path}")
            for line in result:
                print(f"  Line {line[0]}: {line[1]} -> {line[2]}")
    else:
        result = scan_directory_for_markers(args.path, markers, args.ext)
        if result:
            detected = True
            for file, conflicts in result.items():
                print(f"\nMerge conflicts found in: {file}")
                for line in conflicts:
                    print(f"  Line {line[0]}: {line[1]} -> {line[2]}")

    if detected:
        print("Suspect words/markers found.")
        sys.exit(1)
    else:
        print("No suspect words/markers found.")
        sys.exit(0)
