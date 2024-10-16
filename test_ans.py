import argparse
import os
import re
import subprocess
import time


def run_command(cmd):
    status = '?'
    output = ''
    timecost = 0
    try:
        timecost = time.time()
        program = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
        timecost = time.time() - timecost
        output, err = program.communicate(timeout=600)
        output = output.decode('utf-8').strip().replace('[', '').replace(']', '')
        if output == 'no ans':
            output = 'no ans'
        else:
            output = set(output.split(','))
        err = err.decode('utf-8')
        status = 'COMPLETED'
    except subprocess.TimeoutExpired:
        status = 'TIMEOUT'
    except subprocess.CalledProcessError:
        status = 'CRASH'
    return status, output, timecost


def load_ans(result_dir, name):
    name = os.path.splitext(name)[0]
    name = f'{name}.apx-EE-ST.out'
    path = os.path.join(result_dir, 'reference-results', name)

    with open(path, 'r', encoding='utf-8') as f:
        ans = f.read()
    ans = ans.split('\n')[0].strip()
    ans = re.sub(r'(\w+)', r'"\1"', ans)
    ans = eval(ans)
    return ans


def read_input_dir():
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', '--data-dir', type=str, help='directory of the test data')
    args = parser.parse_args()

    return args.data_dir


def main():
    input_dir = read_input_dir()
    for file in os.listdir(os.path.join(input_dir, 'instances')):
        if not file.endswith('.tgf'):
            continue
        print(f'{file}: ')
        filename = os.path.join(input_dir, 'instances', file)
        cmd = f'timeout 601 ./CL4AF/build/main {filename}'
        status, output, timecost = run_command(cmd)
        if status != 'COMPLETED':
            print(f'{status}~')
            continue
        ans_list = load_ans(input_dir, file)

        flag = True if len(ans_list) == 0 and output == 'no ans' else False
        for ans in ans_list:
            if set(ans) == output:
                flag = True
                break
        if flag:
            print('accept')
            print(output)
        else:
            print('wrong answer')
            print(f'output: {output}')
            print(f'answer: {ans_list}')


if __name__ == '__main__':
    main()
