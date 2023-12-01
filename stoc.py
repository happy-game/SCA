import numpy as np
from tqdm import tqdm, trange
import matplotlib.pyplot as plt
import os

# change cwd to ../
os.chdir(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SBox = [0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
        0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
        0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
        0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
        0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
        0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
        0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
        0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
        0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
        0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
        0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
        0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
        0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
        0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
        0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
        0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
        ]
SBox = np.array(SBox)
g_table = np.load('data/g_table.npy')
g_table = np.int64(g_table)

# path = 'data/v2/npy/'
path = 'data/1130_1/npy/'
plains = np.load(path + 'plain.npy')
waves = np.load(path + 'wave.npy')
# keys = np.load(path + 'key.npy')
keys = np.array([0x0d, 0x0e, 0x0f, 0x00, 0x09, 0x0a, 0x0b, 0x0c, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04])

# N = plains.shape[0]  # 全部数据
N = 800000
N1 = int(N * 0.45)  # 用于训练 beta
N2 = int(N * 0.45)  # 用于训练 Rt
N3 = int(N * 0.1)  # 用于攻击
# N3 = 1000

POI_NUM = 20
TIMES = 100
PBAR_LEN = 20


# clear the data
def clear():
    files = os.listdir(path)
    keep_files = ['plain.npy', 'wave.npy', 'key.npy', 'cipher.npy']
    for file in files:
        if file.split('.')[-1] == 'npy' and file not in keep_files:
            os.remove(path + file)

# clear()
def preprocessing(i=0):  # calculate g, g is N * 9, default use first key and first plain
    plain = plains[:, i]
    if len(keys.shape) == 1:
        key = keys[i]
    else:
        key = keys[:, i]
    label = plain ^ key
    label = SBox[label]
    g = g_table[label]
    np.save(path + 'g.npy', g)
    return g



def profiling(i=0):
    try:  # g is N1 * 9, if there's on g, calculate it
        g = np.load(path + 'g.npy', dtype=np.uint64)
    except:
        g = preprocessing(i=i)
    # g = g[:N1, :]
    print('g is ready, shape: ', g.shape, g.dtype)

    try:  # beta is N1 * 9, if there's on beta, calculate it
        beta = np.load(path + 'beta.npy')
    except:
        beta = np.zeros((waves.shape[1], g.shape[1]))   # beta = (A.T * A)^-1 * A.T * wave
        A = g[:N1, :]   # pre calculate (A.T * A)^-1 * A.T
        ATA = np.dot(A.T, A)
        ATA_inv = np.linalg.inv(ATA)
        ATA_inv_AT = np.dot(ATA_inv, A.T)

        for i in trange(waves.shape[1], desc='%20s'%'Calculating beta'):  
            beta[i] = np.dot(ATA_inv_AT, waves[:N1, i])
        np.save(path + 'beta.npy', beta)
    print('beta is ready, shape: ', beta.shape, beta.dtype)

    # ht is the data dependent, it's size is N * wave.shape[1]
    try:
        ht = np.load(path + 'ht.npy')
    except:
        step = 10000    # if N is too big, use step to calculate ht
        ht = np.zeros((waves.shape[0], waves.shape[1]))
        with tqdm(total=waves.shape[0], desc='%20s'%'Calculating ht') as pbar:
            for i in range(0, waves.shape[0], step):
                if i + step > waves.shape[0]:
                    ht[i:] = np.dot(g[i:], beta.T)
                else:
                    ht[i:i + step] = np.dot(g[i:i + step], beta.T)
                pbar.update(step)
        np.save(path + 'ht.npy', ht)
    print('ht is ready, shape: ', ht.shape, ht.dtype)

    # Rt is the noise, it's size is N * wave.shape[1]
    Rt = waves - ht
    np.save(path + 'Rt.npy', Rt)
    print('Rt is ready, shape: ', Rt.shape, Rt.dtype)

    # C is the covariance matrix of Rt, it's size is wave.shape[1] * wave.shape[1], if poi(poing of interest) defined, use it
    try:
        poi = np.load(path + 'poi.npy')
    except:
        t_diff()    # calculate poi
        cal_snr()   # calculate snr
        poi = np.load(path + 'poi.npy')
    C = np.cov(Rt[N1:N1+N2, poi].T)
    C_inv = np.linalg.inv(C)
    np.save(path + 'C.npy', C)
    np.save(path + 'C_inv.npy', C_inv)
    print('C is ready, shape: ', C.shape, C.dtype)


def cal_snr():  # calculate snr,the signal part is ht, the noise part is Rt
    try:
        ht = np.load(path + 'ht.npy')
        Rt = np.load(path + 'Rt.npy')
    except:
        print('ht or Rt not found')
        exit(1)
    ht_var = np.var(ht, axis=0)
    Rt_var = np.var(Rt, axis=0)
    snr = ht_var / Rt_var
    snr = np.abs(snr)

    fig, ax = plt.subplots()
    ax.plot(snr / max(snr))
    ax.set_title('snr')
    ax.set_xlabel('time')
    ax.set_ylabel('snr')
    plt.savefig('figure/snr.png')
    np.save(path + 'snr.npy', snr)
    print('snr is ready, shape: ', snr.shape, snr.dtype)


def t_diff(ind=0):  # use t-test to find the poi
    traces_by_label = []  # label is 0-255
    for i in range(256):
        g = g_table[i]
        traces_by_label.append({
            'label': i,
            'g': g,
            'len': 0,
            'mean': np.zeros((waves.shape[1])),
            'var': np.zeros((waves.shape[1])),
            'ht': np.zeros((waves.shape[1])),
            'traces': []
        })
    if len(keys.shape) == 1:
        label = plains[:, ind] ^ keys[ind]
    else:
        label = plains[:, ind] ^ keys[:, ind]
    # label = plains[:, ind] ^ keys[ind]

    label = SBox[label]
    try:
        beta = np.load(path + 'beta.npy')
    except:
        print('beta not found')
        exit(1)
    for i in trange(N, desc='%20s'%'classifying by label'):  # classify by label
        traces_by_label[label[i]]['traces'].append(waves[i])
        traces_by_label[label[i]]['len'] += 1
    for i in trange(256, desc='%20s'%'Calculating mean'):  # calculate mean, var, ht
        traces_by_label[i]['traces'] = np.array(traces_by_label[i]['traces'])
        traces_by_label[i]['mean'] = np.mean(traces_by_label[i]['traces'], axis=0)
        traces_by_label[i]['var'] = np.var(traces_by_label[i]['traces'], axis=0)
        traces_by_label[i]['ht'] = np.dot(traces_by_label[i]['g'], beta.T)
    t_test = np.zeros((waves.shape[1]))
    for k in trange(waves.shape[1], desc='%20s'%'Calculating t-test'):
        sum = 0  # sum = \sum
        for i in range(256):
            for j in range(i, 256):
                # mi = traces_by_label[i]['mean'][k]  # mean of label i
                # mj = traces_by_label[j]['mean'][k]  # mean of label j
                mi = traces_by_label[i]['ht'][k]  # mean of label i
                mj = traces_by_label[j]['ht'][k]  # mean of label j
                vi = traces_by_label[i]['var'][k]  # var of label i
                vj = traces_by_label[j]['var'][k]  # var of label j
                ni = traces_by_label[i]['len']  # len of label i
                nj = traces_by_label[j]['len']  # len of label j
                sum += ((mi - mj) / np.sqrt((vi ** 2 / ni + vj ** 2 / nj))) ** 2
        t_test[k] = sum
    fig, ax = plt.subplots()
    ax.plot(t_test / max(t_test))
    ax.set_title('t-test')
    ax.set_xlabel('time')
    ax.set_ylabel('t-test')
    plt.savefig('figure/t-test.png')
    np.save(path + 't_test.npy', t_test)
    print('t_test is ready, shape: ', t_test.shape)    # display the top 20 t-test's index and value
    max_index = np.argsort(t_test)[-POI_NUM:]
    max_value = t_test[max_index]
    max_index.sort()
    np.save(path + 'poi.npy', max_index)
    print('poi is ready, shape: ', max_index.shape, max_index.dtype)



def minimum():  # minimum principle
    times = TIMES
    keys_num = 256
    try:
        poi = np.load(path + 'poi.npy')
        beta = np.load(path + 'beta.npy')
        beta_poi = beta[poi, :]
        wave_poi = waves[:, poi]
        g = np.load(path + 'g.npy')
    except Exception as e:
        print(e)
        exit(1)

    all_minimum = np.zeros((times, keys_num))
    for i in trange(times, desc='%20s'%'min guessing key', position=0):
        # random choose N3 traces
        N3_index = np.random.choice(N2, N3, replace=False)
        wave_N3 = wave_poi[N3_index]
        plain_N3 = plains[N3_index, 0]

        # Z_key = np.zeros((N3, beta_N3.shape[1]))
        for key in trange(0, keys_num, position=1, leave=False, desc='%20s'%''):  # guess key, calculate Z
            g = g_table[SBox[plain_N3 ^ key]] # g is N3 * 9
            h = g @ beta_poi.T # h is N3 * len(poi) 
            R = wave_N3 - h
            sum = 0
            for j in range(N3):
                for k in range(len(poi)):
                    sum += R[j][k] ** 2
            sum /= N3
            all_minimum[i][key] = sum
    np.save(path + 'all_min_likelihood.npy', all_minimum)

    fig, ax = plt.subplots()
    ax.plot(all_minimum[0])
    title = 'minimum principle: min is v[%d]=%.3f max is v[%d]=%.3f' % (np.argmin(all_minimum[0]), np.min(all_minimum[0]), np.argmax(all_minimum[0]), np.max(all_minimum[0]))
    ax.set_title(title)
    ax.set_xlabel('key')
    ax.set_ylabel('likelihood')
    plt.savefig('figure/minimum.png')


def maximum():  # maximum likelihood principle
    times = TIMES
    keys_num = 256
    try:
        poi = np.load(path + 'poi.npy')
        beta = np.load(path + 'beta.npy')
        beta_poi = beta[poi, :]
        wave_poi = waves[:, poi]
        g = np.load(path + 'g.npy')
        C = np.load(path + 'C.npy')
        C_inv = np.linalg.inv(C)
    except Exception as e:
        print(e)
        exit(1)

    all_max_likelihood = np.zeros((times, keys_num))
    for i in trange(times, desc='%20s'%'max guessing key', position=0):
        # random choose N3 traces
        N3_index = np.random.choice(N2, N3, replace=False)
        wave_N3 = wave_poi[N3_index]
        plain_N3 = plains[N3_index, 0]

        # Z_key = np.zeros((N3, beta_N3.shape[1]))
        for key in trange(0, keys_num, position=1, leave=False, desc='%20s'%''):  # guess key, calculate Z
            g = g_table[SBox[plain_N3 ^ key]] # g is N3 * 9
            h = g @ beta_poi.T # h is N3 * len(poi) 
            R = wave_N3 - h
            sum = 0
            for j in range(N3):
                sum += R[j].T @ C_inv @ R[j]
            sum /= N3
            all_max_likelihood[i][key] = sum
    np.save(path + 'all_max_likelihood.npy', all_max_likelihood)

    fig, ax = plt.subplots()
    ax.plot(all_max_likelihood[0])
    title = 'maximum likelihood: min is v[%d]=%.3f\ max is v[%d]=%.3f' % (np.argmin(all_max_likelihood[0]), np.min(all_max_likelihood[0]), np.argmax(all_max_likelihood[0]), np.max(all_max_likelihood[0]))
    ax.set_title(title)
    ax.set_xlabel('key')
    ax.set_ylabel('likelihood')
    plt.savefig('figure/maximum likelihood.png')

def cpa():  # correlation power analysis
    times = TIMES
    keys_num = 256
    try:
        poi = np.load(path + 'poi.npy')
        beta = np.load(path + 'beta.npy')
        beta_poi = beta[poi, :]
        wave_poi = waves[:, poi]
        g = np.load(path + 'g.npy')
    except Exception as e:
        print(e)
        exit(1)
    
    all_cpa = np.zeros((times, keys_num, len(poi)))
    for i in trange(times, desc='%20s'%'cpa guessing key', position=0):
        # random choose N3 traces
        N3_index = np.random.choice(N2, N3, replace=False)
        wave_N3 = wave_poi[N3_index]
        plain_N3 = plains[N3_index, 0]

        # Z_key = np.zeros((N3, beta_N3.shape[1]))
        for key in trange(0, keys_num, position=1, leave=False, desc='%20s'%''):  # guess key, calculate Z
            g = g_table[SBox[plain_N3 ^ key]]
            h = g @ beta_poi.T
            R = wave_N3 - h
            for j in range(len(poi)):
                all_cpa[i][key][j] = np.corrcoef(R[:, j], h[:, j])[0, 1]
    all_cpa = np.abs(all_cpa)
    np.save(path + 'all_cpa.npy', all_cpa)

    fig, ax = plt.subplots()
    for i in range(keys_num):
        if i == np.argmax(all_cpa[0]):
            ax.plot(all_cpa[0][i], label=str(i), color='r')
        else:
            ax.plot(all_cpa[0][i], color='gray', alpha=0.5)
    ax.set_title('cpa')
    ax.set_xlabel('time')
    ax.set_ylabel('cpa')
    plt.savefig('figure/cpa.png')


if __name__ == '__main__':
    clear()
    profiling()
    maximum()
    minimum()
    cpa()

