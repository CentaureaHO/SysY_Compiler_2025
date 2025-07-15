def genstr(s):
    asciis = [10] + [ord(c) for c in s] + [10]
    return "\n".join(f"putch({ascii});" for ascii in asciis)

import sys
if len(sys.argv) > 1:
    s = sys.argv[1]
else:
    print("Usage: python genstr.py <string>")
    sys.exit(1)

print(genstr(s))
