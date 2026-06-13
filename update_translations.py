import xml.etree.ElementTree as ET
from pathlib import Path

ts_path = Path(r"D:\Work\RsWorkTools\ToolsRuntime\res\translations\SARibbon_ru.ts")

translations = {
    'SAColorMenu': {
        'None': 'Нет',
        'Theme Colors': 'Цвета темы',
        'Custom Color': 'Пользовательский цвет',
    },
    'SARibbonActionsManager': {
        'not in ribbon': 'не на ленте',
    },
    'SARibbonActionsManagerModel': {
        'action name': 'имя действия',
    },
    'SARibbonCategoryLayout': {
        'in SARibbonCategoryLayout cannot addItem,use addPannel instead': 'в SARibbonCategoryLayout нельзя использовать addItem, используйте addPannel',
    },
    'SARibbonContextCategory': {
        'SARibbonContextCategory have category %1,will skip': 'SARibbonContextCategory уже содержит категорию %1, будет пропущено',
    },
    'SARibbonCustomizeDialog': {
        'Customize Dialog': 'Настройка ленты',
        'Cancel': 'Отмена',
        'OK': 'OK',
    },
    'SARibbonCustomizeWidget': {
        'Customize Widget': 'Настройка ленты',
        'Please Select': 'Выберите',
        'Find Command': 'Поиск команды',
        'Add >>': 'Добавить >>',
        '<< Remove': '<< Удалить',
        'Customize the Ribbon': 'Настройка ленты',
        'Main Category': 'Основные вкладки',
        'All Category': 'Все вкладки',
        'New Category': 'Новая вкладка',
        'New Group': 'Новая группа',
        'Rename': 'Переименовать',
        'reset': 'Сброс',
        'proportion:': 'пропорция:',
        'large': 'большой',
        'small': 'маленький',
        'medium': 'средний',
        'new category[customize]%1': 'новая вкладка[настройка]%1',
        'new pannel[customize]%1': 'новая группа[настройка]%1',
        'rename': 'переименование',
        'name:': 'имя:',
        'Warning': 'Предупреждение',
        'Are you sure reset all customize setting?': 'Вы уверены, что хотите сбросить все настройки?',
    },
    'SARibbonSystemButtonBar': {
        'Restore': 'Восстановить',
        'Maximize': 'Развернуть',
    },
    'SARibbonToolButton': {
        'The TextEllipsisAspectFactor parameter cannot be set to 0 or a negative number': 'Параметр TextEllipsisAspectFactor не может быть 0 или отрицательным числом',
    },
}

# XML with original declaration
text = ts_path.read_text(encoding='utf-8')

# Register namespace to avoid ns0 prefix
ET.register_namespace('', 'urn:qt:ts')

tree = ET.parse(str(ts_path))
root = tree.getroot()

for context in root.findall('context'):
    name_elem = context.find('name')
    if name_elem is None:
        continue
    ctx_name = name_elem.text
    ctx_trans = translations.get(ctx_name, {})
    if not ctx_trans:
        continue

    for message in context.findall('message'):
        source = message.find('source')
        if source is None or source.text is None:
            continue
        src_text = source.text
        if src_text in ctx_trans:
            translation = message.find('translation')
            if translation is None:
                translation = ET.SubElement(message, 'translation')
            translation.text = ctx_trans[src_text]
            # Remove type attribute if present
            if 'type' in translation.attrib:
                del translation.attrib['type']

# Write preserving XML declaration and encoding
tree.write(str(ts_path), encoding='utf-8', xml_declaration=True)
print("Translations updated.")
