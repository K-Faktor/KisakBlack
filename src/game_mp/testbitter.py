import re
import sys
from pathlib import Path


# ----------- PATTERNS -----------

# bitarray<N>::testBit(&expr, value)
bitarray_pattern = re.compile(
    r'bitarray<\d+>::testBit\(\s*&([^,]+?)\s*,\s*([^)]+?)\s*\)'
)

# EntHandle::isDefined(&expr)
isdefined_pattern = re.compile(
    r'EntHandle::isDefined\(\s*&([^)]+?)\s*\)'
)

# EntHandle::ent(&expr)
ent_pattern = re.compile(
    r'EntHandle::ent\(\s*&([^)]+?)\s*\)'
)


# ----------- REPLACERS -----------

def replace_bitarray(match):
    expr = match.group(1).strip()
    value = match.group(2).strip()

    # Remove trailing u/U from numeric literal
    value = re.sub(r'[uU]\b', '', value)

    return f"{expr}.testBit({value})"


def replace_isdefined(match):
    expr = match.group(1).strip()
    return f"{expr}.isDefined()"


def replace_ent(match):
    expr = match.group(1).strip()
    return f"{expr}.ent()"


# ----------- PROCESSING -----------

def process_file(path: Path):
    text = path.read_text(encoding="utf-8")

    text, count1 = bitarray_pattern.subn(replace_bitarray, text)
    text, count2 = isdefined_pattern.subn(replace_isdefined, text)
    text, count3 = ent_pattern.subn(replace_ent, text)

    total = count1 + count2 + count3

    if total > 0:
        path.write_text(text, encoding="utf-8")
        print(f"Replaced {total} occurrences in {path}")
        print(f"  bitarray: {count1}")
        print(f"  isDefined: {count2}")
        print(f"  ent: {count3}")
    else:
        print(f"No matches found in {path}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python fix_calls.py <file.cpp>")
        sys.exit(1)

    file_path = Path(sys.argv[1])

    if not file_path.exists():
        print("File not found.")
        sys.exit(1)

    process_file(file_path)