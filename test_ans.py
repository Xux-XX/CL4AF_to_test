import argparse
import json
import os
import re
import subprocess
import time


def run_command(cmd):
    status = '?'
    output = ''
    timecost = 0
    try:
        timecost = time.perf_counter()
        program = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
        timecost = time.perf_counter() - timecost
        output, err = program.communicate(timeout=600)
        output = output.decode('utf-8').strip().replace('[', '').replace(']', '')
        if output == 'no ans':
            output = 'no ans'
        else:
            output = list(output.split(','))
        status = 'COMPLETED'
    except subprocess.TimeoutExpired:
        status = 'TIMEOUT'
    except subprocess.CalledProcessError:
        status = 'CRASH'
    return status, output, round(timecost * 1000)


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
    is_first = True
    input_dir = read_input_dir()
    print('[')
    for file in os.listdir(os.path.join(input_dir, 'instances')):
        if not file.endswith('.tgf'):
            continue
        filename = os.path.join(input_dir, 'instances', file)
        cmd = f'timeout 601 ./CL4AF/build/main {filename}'
        status, output, timecost = run_command(cmd)
        ans_list = load_ans(input_dir, file)
        record = dict(
            file=filename,
            status='?',
            timecost=f'{timecost} ms',
            output='',
            answer_list=''
        )

        if status != 'COMPLETED':
            record['status'] = status
        else:
            flag = True if len(ans_list) == 0 and output == 'no ans' else False
            for ans in ans_list:
                if set(ans) == set(output):
                    flag = True
                    break
            if flag:
                record['status'] = 'ACCEPT'
            else:
                record['status'] = "WRONG_ANSWER"

        if not is_first:
            print(',')
        is_first = False
        record['output'] = output
        record['answer_list'] = ans_list
        print(json.dumps(record, ensure_ascii=False, indent=4))
    print(']')
    print('(-_-)zzz')


if __name__ == '__main__':
    main()