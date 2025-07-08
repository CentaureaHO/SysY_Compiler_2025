import argparse
import subprocess
import os
import re
import time

def execute(command):
    return subprocess.run(command, capture_output=True, text=True)

def execute_with_stdin_out(command):
    return os.system(command) % 255

def check_file(file1, file2):
    try:
        result = execute(["diff", "--strip-trailing-cr", file1, file2, "-b"])
    except Exception as e:
        print(f"\033[91mUnknown Error on \033[0m{file1}, \033[91mPlease check your output file\033[0m")
        return 1
    else:
        return result.returncode

def add_returncode(file, ret):
    need_newline = False
    with open(file, "r") as f:
        try:
            content = f.read()
        except Exception as e:
            print(f"\033[91mUnknown Error on \033[0m{file}, \033[91mPlease check your output file\033[0m")
            return False
        else:
            if len(content) > 0:
                if not content.endswith("\n"):
                    need_newline = True

    with open(file, "a+") as f:
        if need_newline:
            f.write("\n")
        f.write(str(ret))
        f.write("\n")
    return False

def parse_time(time_str):
    match = re.search(r'TOTAL:\s*(\d+)H-(\d+)M-(\d+)S-(\d+)us', time_str)
    if match:
        hours, minutes, seconds, microseconds = map(int, match.groups())
        total_microseconds = hours * 3600 * 1000000 + minutes * 60 * 1000000 + seconds * 1000000 + microseconds
        return total_microseconds
    return None

def execute_ir_performance(input_file, output_file, opt_arg, stdin_file, stdout_file, testout_file):
    result = execute(["timeout", "10", "./bin/compiler", "-llvm", "-o", output_file, input_file, opt_arg])
    if result.returncode != 0:
        print(f"\033[93mCompile Error on \033[0m{input_file}")
        return 0, None
    
    result = execute(["clang", output_file, "-c", "-o", "tmp.o", "-w"])
    if result.returncode != 0:
        print(f"\033[93mOutPut Error on \033[0m{input_file}")
        return 0, None
        
    result = execute(["clang", "-static", "tmp.o", "-L./lib", "-lsysy_x86"])
    if result.returncode != 0:
        execute(["rm", "-rf", "tmp.o"])
        print(f"\033[93mLink Error on \033[0m{input_file}")
        return 0, None
    
    execute(["rm", "-rf", "tmp.o"])

    result = 0
    if stdin_file == "none":
        result = execute_with_stdin_out(f"timeout 30 ./a.out > {testout_file} 2>performance_time.tmp")
    else:
        result = execute_with_stdin_out(f"timeout 30 ./a.out < {stdin_file} > {testout_file} 2>performance_time.tmp")
    
    if result == 124:
        print(f"\033[93mTime Limit Exceed on \033[0m{input_file}")
        return 0, None
    elif result == 139:
        print(f"\033[95mRunTime Error on \033[0m{input_file}")
        return 0, None

    time_us = None
    try:
        with open("performance_time.tmp", "r") as f:
            time_output = f.read()
            time_us = parse_time(time_output)
    except:
        pass
    
    add_returncode(testout_file, result)
    
    if check_file(testout_file, stdout_file) == 0:
        print(f"\033[92mAccept \033[0m{input_file}")
        return 1, time_us
    else:
        print(f"\033[91mWrong Answer on \033[0m{input_file}")
        return 0, time_us

def execute_asm_performance(input_file, output_file, opt_arg, stdin_file, stdout_file, testout_file):
    result = execute(["timeout", "60", "./bin/compiler", "-S", "-o", output_file, input_file, opt_arg])
    if result.returncode != 0:
        print(f"\033[93mCompile Error on \033[0m{input_file}")
        return 0, None
    
    result = execute(["riscv64-unknown-linux-gnu-gcc", output_file, "-c", "-o", "tmp.o", "-w"])
    if result.returncode != 0:
        print(f"\033[93mOutPut Error on \033[0m{input_file}")
        return 0, None

    result = execute(["riscv64-unknown-linux-gnu-gcc", "-static", "tmp.o", "-L./lib", "-lsysy_riscv", "-mcmodel=medany"])
    if result.returncode != 0:
        execute(["rm", "-rf", "tmp.o"])
        print(f"\033[93mLink Error on \033[0m{input_file}")
        return 0, None
    
    execute(["rm", "-rf", "tmp.o"])
    
    result = 0
    if stdin_file == "none":
        result = execute_with_stdin_out(f"timeout 60 qemu-riscv64 ./a.out > {testout_file} 2>performance_time.tmp")
    else:
        result = execute_with_stdin_out(f"timeout 60 qemu-riscv64 ./a.out < {stdin_file} > {testout_file} 2>performance_time.tmp")
    
    if result == 124:
        print(f"\033[93mTime Limit Exceed on \033[0m{input_file}")
        return 0, None
    elif result == 139:
        print(f"\033[95mRunTime Error on \033[0m{input_file}")
        return 0, None
    
    time_us = None
    try:
        with open("performance_time.tmp", "r") as f:
            time_output = f.read()
            time_us = parse_time(time_output)
    except:
        pass
    
    add_returncode(testout_file, result)
    
    if check_file(testout_file, stdout_file) == 0:
        print(f"\033[92mAccept \033[0m{input_file}")
        return 1, time_us
    else:
        print(f"\033[91mWrong Answer on \033[0m{input_file}")
        return 0, time_us

def format_time(time_us):
    if time_us is None:
        return "N/A"
    
    if time_us >= 1000000:
        return f"{time_us / 1000000:.3f}s"
    elif time_us >= 1000:
        return f"{time_us / 1000:.3f}ms"
    else:
        return f"{time_us}us"

def load_previous_results(filepath):
    if not os.path.exists(filepath):
        return {}
    
    previous_results = {}
    try:
        with open(filepath, 'r') as f:
            lines = f.readlines()
            for line in lines[1:]:
                parts = line.strip().split(',')
                if len(parts) >= 3:
                    test_name = parts[0]
                    time_us_str = parts[2]
                    try:
                        time_us = int(time_us_str) if time_us_str else None
                        previous_results[test_name] = time_us
                    except ValueError:
                        previous_results[test_name] = None
    except Exception:
        pass
    
    return previous_results

def calculate_improvement(current_time, previous_time):
    if current_time is None or previous_time is None:
        return "N/A"
    
    if previous_time == 0:
        return "N/A"
    
    improvement = ((previous_time - current_time) / previous_time) * 100
    if improvement > 0:
        return f"+{improvement:.1f}%"
    else:
        return f"{improvement:.1f}%"

def main():
    parser = argparse.ArgumentParser(description="Performance test for riscv_optimizer",
                                   epilog="Example: python3 performance.py asm 2 --save-performance")
    parser.add_argument('target', choices=['asm', 'ir'], help='Test target: asm or ir')
    parser.add_argument('opt_level', type=int, help='Optimization level')
    parser.add_argument('--save-performance', action='store_true', 
                       help='Update baseline performance results for comparison')
    
    args = parser.parse_args()
    
    input_folder = "testcase/riscv_optimizer"
    
    if args.target == "asm":
        output_folder = "test_output/riscv_optimizerASM"
    else:
        output_folder = "test_output/riscv_optimizerIR"
    
    opt_arg = f"-O{args.opt_level}"
    
    os.makedirs(output_folder, exist_ok=True)
    
    results = []
    ac = 0
    total = 0
    
    sy_files = [f for f in os.listdir(input_folder) if f.endswith(".sy")]
    sy_files.sort()
    
    for file in sy_files:
        total += 1
        name = file.split(".")[0]
        input_file = os.path.join(input_folder, file)
        stdin_file = os.path.join(input_folder, f"{name}.in")
        stdout_file = os.path.join(input_folder, f"{name}.out")
        
        if args.target == "ir":
            output_file = os.path.join(output_folder, f"{name}.ll")
        else:
            output_file = os.path.join(output_folder, f"{name}.s")
        
        if not os.path.exists(stdin_file):
            stdin_file = "none"

        if args.target == "ir":
            passed, time_us = execute_ir_performance(input_file, output_file, opt_arg, 
                                                   stdin_file, stdout_file, "tmp.out")
        else:
            passed, time_us = execute_asm_performance(input_file, output_file, opt_arg, 
                                                    stdin_file, stdout_file, "tmp.out")
        
        if passed:
            ac += 1
        
        results.append({
            'name': name,
            'passed': passed,
            'time_us': time_us,
            'time_str': format_time(time_us)
        })
    
    performance_dir = "test_output/basic_performance"
    os.makedirs(performance_dir, exist_ok=True)
    filename = f"{args.target}_baseline.csv"
    filepath = os.path.join(performance_dir, filename)

    baseline_results = load_previous_results(filepath)

    print(f"\n{'='*100}")
    print(f"Performance Test Results - {args.target.upper()} O{args.opt_level}")
    if baseline_results:
        print(f"Compared to baseline from: {filepath}")
    print(f"{'='*100}")
    if baseline_results:
        print(f"{'Test Case':<30} {'Status':<10} {'Time':<15} {'Baseline':<15} {'Improvement':<15}")
        print(f"{'-'*100}")
        
        for result in results:
            status = "PASS" if result['passed'] else "FAIL"
            status_color = "\033[92m" if result['passed'] else "\033[91m"
            baseline_time = baseline_results.get(result['name'])
            baseline_time_str = format_time(baseline_time) if baseline_time else "N/A"
            improvement = calculate_improvement(result['time_us'], baseline_time)
            
            if improvement != "N/A":
                if improvement.startswith('+'):
                    improvement_color = "\033[92m"
                elif improvement.startswith('-'):
                    improvement_color = "\033[91m"
                else:
                    improvement_color = "\033[0m"
                improvement_display = f"{improvement_color}{improvement}\033[0m"
            else:
                improvement_display = improvement
            
            print(f"{result['name']:<30} {status_color}{status:<10}\033[0m {result['time_str']:<15} {baseline_time_str:<15} {improvement_display:<15}")
    else:
        print(f"{'Test Case':<30} {'Status':<10} {'Time':<15}")
        print(f"{'-'*80}")
        
        for result in results:
            status = "PASS" if result['passed'] else "FAIL"
            status_color = "\033[92m" if result['passed'] else "\033[91m"
            print(f"{result['name']:<30} {status_color}{status:<10}\033[0m {result['time_str']:<15}")
    
    print(f"{'-'*100}")
    print(f"Total: {ac}/{total} passed")
    
    valid_times = [r['time_us'] for r in results if r['time_us'] is not None and r['passed']]
    if valid_times:
        avg_time = sum(valid_times) / len(valid_times)
        max_time = max(valid_times)
        min_time = min(valid_times)
        print(f"Average time: {format_time(avg_time)}")
        print(f"Max time: {format_time(max_time)}")
        print(f"Min time: {format_time(min_time)}")

        if baseline_results:
            prev_valid_times = [baseline_results.get(r['name']) for r in results 
                               if r['time_us'] is not None and r['passed'] and baseline_results.get(r['name']) is not None]
            if prev_valid_times:
                prev_avg_time = sum(prev_valid_times) / len(prev_valid_times)
                overall_improvement = calculate_improvement(avg_time, prev_avg_time)
                improvement_color = ""
                if overall_improvement != "N/A":
                    if overall_improvement.startswith('+'):
                        improvement_color = "\033[92m"
                    elif overall_improvement.startswith('-'):
                        improvement_color = "\033[91m"
                print(f"Overall improvement: {improvement_color}{overall_improvement}\033[0m")

    if args.save_performance:
        with open(filepath, 'w') as f:
            f.write("TestCase,Status,Time(us),Time(formatted)\n")
            for result in results:
                status = "PASS" if result['passed'] else "FAIL"
                time_us = result['time_us'] if result['time_us'] is not None else ""
                f.write(f"{result['name']},{status},{time_us},{result['time_str']}\n")
        
        print(f"Baseline performance results updated in: {filepath}")
    
    for tmp_file in ["tmp.out", "a.out", "performance_time.tmp"]:
        if os.path.exists(tmp_file):
            os.remove(tmp_file)

if __name__ == "__main__":
    main()
