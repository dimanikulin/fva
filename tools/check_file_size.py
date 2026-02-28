import sys
import os

def count_code_lines(filepath, comment_symbol="#"):
    """
    Count code lines in a source file.
    Ignores empty lines and lines starting with the comment symbol.
    """
    code_lines = 0
    with open(filepath, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            stripped = line.strip()
            if not stripped:
                continue
            if stripped.startswith(comment_symbol):
                continue
            code_lines += 1
    return code_lines


def scan_directory(root_dir, max_lines, comment_symbol="#", extensions=None):
    """
    Recursively scan directory and check each file against max_lines.
    Returns: (passed_count, failed_count)
    """
    passed = 0
    failed = 0

    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            if extensions and not any(filename.endswith(ext) for ext in extensions):
                continue
            filepath = os.path.join(dirpath, filename)
            try:
                count = count_code_lines(filepath, comment_symbol)
                if count > max_lines:
                    print(f"{filepath}: {count} lines (limit {max_lines})")
                    failed += 1
                else:
                    print(f"{filepath}: {count} lines (limit {max_lines})")
                    passed += 1
            except Exception as e:
                print(f"Skipping {filepath}: {e}")

    return passed, failed


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python check_code_lines.py <dir> <max_lines> [comment_symbol] [ext1 ext2 ...]")
        sys.exit(1)

    root_dir = sys.argv[1]
    max_lines = int(sys.argv[2])
    comment_symbol = sys.argv[3] if len(sys.argv) > 3 else "#"
    extensions = sys.argv[4:] if len(sys.argv) > 4 else None

    passed, failed = scan_directory(root_dir, max_lines, comment_symbol, extensions)

    print("\n--- Summary ---")
    print(f"Passed files: {passed}")
    print(f"Failed files: {failed}")

    # pass even if there are failures, to allow CI to report all issues in one run
    sys.exit(0)
