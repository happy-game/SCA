import numpy as np
from tqdm import tqdm
import os

read_paths = ['data/1130_1/', 'data/1130_2/']

for read_path in read_paths:
    print('Reading ' + read_path)
    raw_path = read_path + 'raw/'
    npy_path = read_path + 'npy/'
    txt_files = os.listdir(raw_path)

    for txt_file in txt_files:
        txt_file = txt_file
        if txt_file.split('.')[-1] != 'txt':
            continue
        datas = []
        with open(raw_path + txt_file, 'r') as f:
            file_size = os.path.getsize(raw_path + txt_file)
            pbar = tqdm(total=file_size, desc='Reading ' + txt_file)
            for line in f:
                pbar.update(len(line))
                line_num = line.strip().split()
                line_num = [int(x, base=16) for x in line_num]
                data = np.array(line_num)
                datas.append(data)
            datas = np.array(datas)
        np.save(npy_path + txt_file[:-4], datas)


        