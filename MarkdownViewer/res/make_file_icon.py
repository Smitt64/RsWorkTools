# -*- coding: utf-8 -*-
# Генератор файловой иконки MarkdownViewer: res/markdown-file-icon.ico
# Дизайн соответствует res/markdown-file-icon.svg (лист с загнутым углом и буквой "M")
# ICO используется инсталлятором как иконка ассоциации .md/.markdown файлов
import os
from PIL import Image, ImageDraw

INDIGO = (91, 95, 199, 255)      # #5B5FC7 - основной цвет приложения
FOLD = (194, 196, 239, 255)      # #C2C4EF - загнутый угол (светлый индиго)
SHEET = (255, 255, 255, 255)     # #ffffff - лист

SUPERSAMPLE = 8
ICON_SIZES = [16, 24, 32, 48, 64, 256]

# Геометрия в координатах 16x16 (как в SVG)
SHEET_POLY = [(2.5, 1.5), (10.5, 1.5), (13.5, 4.5), (13.5, 14.5), (2.5, 14.5)]
FOLD_POLY = [(10.5, 1.5), (10.5, 4.5), (13.5, 4.5)]
M_POINTS = [(4.5, 11.5), (4.5, 6.0), (8.0, 10.0), (11.5, 6.0), (11.5, 11.5)]
OUTLINE_W = 1.0
M_W = 1.3


def render(size):
    s = size * SUPERSAMPLE
    k = s / 16.0
    img = Image.new("RGBA", (s, s), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)

    scale = lambda pts: [(x * k, y * k) for x, y in pts]

    # Лист с загнутым углом
    d.polygon(scale(SHEET_POLY), fill=SHEET, outline=INDIGO,
              width=max(1, round(OUTLINE_W * k)))

    # Загнутый угол
    d.polygon(scale(FOLD_POLY), fill=FOLD, outline=INDIGO,
              width=max(1, round(OUTLINE_W * k)))

    # Буква "M"
    d.line(scale(M_POINTS), fill=INDIGO, width=max(1, round(M_W * k)),
           joint="curve")

    return img.resize((size, size), Image.LANCZOS)


def main():
    here = os.path.dirname(os.path.abspath(__file__))

    ico_path = os.path.join(here, "markdown-file-icon.ico")
    # ICO сохраняем из самого большого рендера: Pillow сам даунскейлит до sizes
    render(max(ICON_SIZES)).save(ico_path, format="ICO",
                                 sizes=[(s, s) for s in ICON_SIZES])
    print("written:", ico_path)

    # Превью для визуальной проверки
    preview_path = os.path.join(here, "markdown-file-icon-preview.png")
    render(128).save(preview_path)
    print("written:", preview_path)


if __name__ == "__main__":
    main()
