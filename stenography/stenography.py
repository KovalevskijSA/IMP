from PIL import Image
import numpy as np
from skimage import io
from skimage.util import view_as_blocks
from scipy.fftpack import dct, idct
import random

u1, v1 = 3, 4
u2, v2 = 4, 3
N = 8
P = 25
color_index = 2

def to_bits(s):
    result = []
    for c in s:
        bits = bin(ord(c))[2:]
        result.extend(map(int, bits.rjust(8, '0')))
    return result


def from_bits(bits):
    chars = []
    for i in range(len(bits) // 8):
        byte_char = bits[i * 8:(i + 1) * 8]
        int_char = int(''.join(map(str, byte_char)), 2)
        chars.append(chr(int_char))
    return ''.join(chars)



def abs_diff_coeffs(enc_block):
    return abs(enc_block[u1, v1]) - abs(enc_block[u2, v2])


def is_valid_coeff(enc_block, bit, max_level):
    difference = abs_diff_coeffs(enc_block)
    if (bit == 0) and (difference > max_level):
        return True
    elif (bit == 1) and (difference < -max_level):
        return True
    else:
        return False


def inc_abs(x):
    return x + 1 if x >= 0 else x - 1


def deс_abs(x):
    if np.abs(x) <= 1:
        return 0
    else:
        return x - 1 if x >= 0 else x + 1


def change_coeffs(enc_block, bit):
    coefs = enc_block.copy()
    if bit == 0:
        coefs[u1, v1] = inc_abs(coefs[u1, v1])
        coefs[u2, v2] = deс_abs(coefs[u2, v2])
    elif bit == 1:
        coefs[u1, v1] = deс_abs(coefs[u1, v1])
        coefs[u2, v2] = inc_abs(coefs[u2, v2])
    return coefs


def round_to_byte(arr):
    return np.uint8(np.round(np.clip(arr, 0, 255), 0))


def encodeInfo(img, msg):
    img = img.copy()
    color = img[:, :, color_index]
    blocks = view_as_blocks(color, block_shape=(N, N))
    
    w = blocks.shape[1]
    h = blocks.shape[0]
    r_max = w*h
    encBlockList = []
    iter = 0
    for bit in msg:
        block_number = random.randrange(r_max - 1)
        while(block_number in encBlockList):
            block_number = random.randrange(r_max - 1)
        iter += 1 
        if iter > 0.9*w*h:
            print("err")
            break
        i = block_number // w
        j = block_number % w
        encBlockList.append(block_number)
        block = blocks[i, j]
        coefs = dct(dct(block, axis=0, norm='ortho'), axis=1, norm='ortho')
        while not is_valid_coeff(coefs, bit, P) or (bit != get_bit(block)):
            coefs = change_coeffs(coefs, bit)
            block = round_to_byte(idct(idct(coefs, axis=0, norm='ortho'), axis=1, norm='ortho'))
        color[i * N: (i + 1) * N, j * N: (j + 1) * N] = block
    img[:, :, color_index] = color
    return img, encBlockList


def get_bit(block):
    enc_block = dct(dct(block, axis=0, norm='ortho'), axis=1, norm='ortho')
    return 0 if abs_diff_coeffs(enc_block) > 0 else 1


def decodeInfo(img, encBlockList):
    blocks = view_as_blocks(img[:, :, color_index], block_shape=(N, N))
    h = blocks.shape[1]
    return [get_bit(blocks[i//h, i % h]) for i in encBlockList]


if __name__ == "__main__":
    input_file = "husky.jpg"
    output_file = "output.jpg"
    img = io.imread(input_file)

    msg = "Steganography is the practice of concealing a file, message, image, or video within another file, message, image, or video. The word steganography combines the Greek words steganos, meaning covered or concealed, and graphe meaning writing"
    print(f"Message: {msg}")
    test_message = to_bits(msg)

    cimg, blocks_list = encodeInfo(img, test_message)
    
    print(len(test_message))
    print(len(blocks_list))

    msg = decodeInfo(cimg, blocks_list)
    dmsg = from_bits(msg)
    print(f"Decode message: {dmsg}")
    
    io.imsave(output_file, cimg)
    f = open("list_of_blocks.txt", "a")
    f.write(str(blocks_list))
    f.close()