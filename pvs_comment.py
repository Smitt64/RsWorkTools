import os

def add_comment_to_files(directory):
    file_extensions = ['.c', '.cpp', '.h', '.hpp']
    comment = '// This is an independent project of an individual developer. Dear PVS-Studio, please check it.\n// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com'

    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(tuple(file_extensions)):
                file_path = os.path.join(root, file)
                with open(file_path, 'r') as f:
                    content = f.read()
                    if comment not in content:
                        with open(file_path, 'a') as f:
                            f.write('\n' + comment)

# Пример вызова функции для текущей директории и всех поддиректорий
add_comment_to_files('.')
