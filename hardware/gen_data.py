import colorsys

FRAME_NUMBER = 500
LED_NUMBER = 8     # 每個 channel 有 8 顆 LED
CHANNEL_NUMBER = 8   # 兩個 channel
COLOR_COUNT = 49

# 建立彩虹色表
rainbow_table = []
for i in range(COLOR_COUNT):
    hue = i / COLOR_COUNT
    r, g, b = colorsys.hsv_to_rgb(hue, 1.0, 1.0)
    r = int(r * 31)
    g = int(g * 31)
    b = int(b * 31)
    rainbow_table.append([r, g, b])

# 產生所有 frame
frames = []
for frame_idx in range(FRAME_NUMBER - 1):
    frame = []
    for ch in range(CHANNEL_NUMBER):
        ch_leds = []
        for led_idx in reversed(range(LED_NUMBER)):
            color_idx = (led_idx + frame_idx + 3*ch) % COLOR_COUNT
            ch_leds.append(rainbow_table[color_idx])
        frame.append(ch_leds)
    frames.append(frame)

# 最後一幀全黑
black_frame = [[[0, 0, 0] for _ in range(LED_NUMBER)] for _ in range(CHANNEL_NUMBER)]
frames.append(black_frame)

# 輸出 C 檔案
with open(r"main/led_table.c", "w") as f:
    f.write("#include <stdint.h>\n")
    f.write(f"#define FRAME_NUMBER {FRAME_NUMBER}\n")
    f.write(f"#define CHANNEL_NUMBER {CHANNEL_NUMBER}\n")
    f.write(f"#define LED_NUMBER {LED_NUMBER}\n\n")
    f.write(f"static uint8_t ws2812b_pixels[FRAME_NUMBER][CHANNEL_NUMBER][LED_NUMBER][3] = {{\n")

    for frame in frames:
        f.write("    {\n")
        for ch_leds in frame:
            f.write("        {\n")
            for rgb in ch_leds:
                f.write(f"            {{{rgb[1]:3d}, {rgb[0]:3d}, {rgb[2]:3d}}},\n")
            f.write("        },\n")
        f.write("    },\n")
    f.write("};\n\n")

print("✅ 已輸出為 led_table[frame][channel][led][RGB] 格式到 main/led_table.c")
