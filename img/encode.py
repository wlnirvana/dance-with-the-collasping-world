from PIL import Image

GIF_PATH = './dancer.gif'
num_frames = 34

all_frames = []
with Image.open(GIF_PATH) as im:
    for i in range(num_frames):
        # read next frame
        im.seek(i)

        # new_im is to pad the original image to 1:2 aspect ratio
        new_im = Image.new("RGB", (290, 580), (255, 255, 255))
        # pasting the original image at (0,90) roughly centers the dancer
        new_im.paste(im, (0, 90))
        # new_im.save('./%02d-padded.jpg' % i)

        # black_white_im is black-white (0 or 255) image, not even grayscale
        black_white_im = new_im.convert('L').point(
            lambda x: 0 if x < 10 else 255, mode='1')
        # binary_im.save('./%02d-binary.jpg' % i)

        # resized_im is the image shrinked to fit into the OLED screen
        resized_im = black_white_im.resize((80, 160))
        # resized_im.save('./%02d-resized.jpg' % i)

        # rotated_img is image rotated to fit into 160w x 80h screen
        rotated_img = resized_im.transpose(Image.ROTATE_90)
        # rotated_img.save('./%02d-rotated.jpg' % i)

        # convert image from 2D to 1D array
        flat_im = list(rotated_img.getdata())
        # map 255 to 1
        binary_im = list(map(lambda x: 1 if x == 255 else 0, flat_im))
        # group every 8 pixels to construct a byte in the future
        grouped_im = [binary_im[i:i+8] for i in range(0, len(binary_im), 8)]

        # collect the groups
        all_frames.append(grouped_im)

# flatten the 34 gif frames into a single list
flatten = [group for frame in all_frames for group in frame]

# ints is the compressed binary image by denoting 8 pixels with only 1 byte
ints = list(map(lambda tup:
                (tup[0] << 7) | (tup[1] << 6) | (tup[2] << 5) | (tup[3] << 4) |
                (tup[4] << 3) | (tup[5] << 2) | (tup[6] << 1) | tup[7],
                flatten))

bytes_str = list(map(lambda byte: '0x%02X,' % byte, ints))
# group every 16 bytes together to form a line
grouped_bytes = [bytes_str[i: i + 16] for i in range(0, len(bytes_str), 16)]
lines = map(lambda arg: ''.join(arg), grouped_bytes)
result = '\n'.join(lines)
with open('./result.txt', 'w') as f:
    f.write(result)