
log_file = 'log/2024_1006.log'

with open(log_file, 'r', encoding='utf-8') as f:
    text = f.read()
tot = text.count('tgf:')
ac = text.count("accept")
wa = text.count('wrong')
rate = round(ac / tot, 3)
print(f'总个数:{tot}')
print(f'正确个数:{ac}')
print(f'错误个数:{wa}')
print(f'正确率:{rate}')
