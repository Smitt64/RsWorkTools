import os

excludes = ['ToolsRuntime\\rstools']
file_extensions = ['.c', '.cpp', '.h', '.hpp']
file_prifixexclude = ['ui_', 'moc_', 'qrc_']

def checkfilename(root : str, file : str):
    for exclude in excludes:
        if exclude in root:
            return False
        
    if not file.endswith(tuple(file_extensions)):
        return False
    
    if file.startswith(tuple(file_prifixexclude)):
        return False
    
    return True

def add_comment_to_files(directory):
    comment = '// This is an independent project of an individual developer. Dear PVS-Studio, please check it.\n// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com'

    for root, _, files in os.walk(directory):
        for file in files:
            if checkfilename(root, file):
                file_path = os.path.join(root, file)
                with open(file_path, 'r', encoding='utf-8') as f:
                    content = f.read()
                    if comment not in content:
                        with open(file_path, 'w' , encoding='utf-8') as f:
                            f.write(comment + '\r\n' + content)

# Пример вызова функции для текущей директории и всех поддиректорий
add_comment_to_files('.')
