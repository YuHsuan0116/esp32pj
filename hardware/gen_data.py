import colorsys

FRAME_NUMBER = 1000
LED_NUMBER = 8
COLOR_COUNT = 200

# 建立彩虹色表
rainbow_table = []
for i in range(COLOR_COUNT):
    hue = i / COLOR_COUNT
    r, g, b = colorsys.hsv_to_rgb(hue, 1.0, 1.0)
    r = int(r * 63)
    g = int(g * 63)
    b = int(b * 63)
    rainbow_table.append([r, g, b])

# 產生所有 frame
frames = []
for frame_idx in range(FRAME_NUMBER - 1):
    frame = []
    for led_idx in reversed(range(LED_NUMBER)):
        color_idx = (led_idx + frame_idx) % COLOR_COUNT
        frame.append(rainbow_table[color_idx])
    frames.append(frame)

# 最後一幀全黑
frames.append([[0, 0, 0] for _ in range(LED_NUMBER)])

# 輸出 C 檔案
with open(r"main/led_table.c", "w") as f:
    f.write("#include <stdint.h>\n")
    f.write(f"#define FRAME_NUMBER {FRAME_NUMBER}\n")
    f.write(f"#define LED_NUMBER {LED_NUMBER}\n\n")
    f.write(f"static uint8_t ws2812_pixels[FRAME_NUMBER][LED_NUMBER][3] = {{\n")

    for frame in frames:
        f.write("    {\n")
        for rgb in frame:
            f.write(f"        {{{rgb[0]:3d}, {rgb[1]:3d}, {rgb[2]:3d}}},\n")
        f.write("    },\n")

    f.write("};\n\n")

print("✅ 已輸出為三維格式到 main/led_table.c")
