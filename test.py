import sys
import subprocess
import os

def handle_lexer():
    bin_path = 'testcase/build/1_lexicalAnalyzer/lexAnalyzer'
    if not os.path.exists(bin_path):
        make_lexer = subprocess.run(["make", "testLexicalAnalysis"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        print(make_lexer.stdout.decode("utf-8"))
    syfiles_path = 'testcase/case/1_lexicalAnalyzer'
    syfiles = [f for f in os.listdir(syfiles_path) if f.endswith('.sy')]
    for file in syfiles:
        print(f'Running {file}...')
        subprocess.run([bin_path, f'{syfiles_path}/{file}'])
        print()

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 test.py <command>")
        print("Commands:")
        print("  lexer")
        sys.exit(1)

    command = sys.argv[1]

    if command == "lexer":
        handle_lexer()
    else:
        print(f"Unknown command: {command}")
        sys.exit(1)

if __name__ == "__main__":
    main()
