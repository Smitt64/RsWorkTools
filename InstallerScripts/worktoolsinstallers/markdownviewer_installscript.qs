function Component()
{
}

Component.prototype.createOperations = function()
{
    // Стандартные операции (распаковка данных компонента)
    component.createOperations();

    if (installer.value("os") === "win")
    {
        // Регистрация MarkdownViewer как просмотрщика Markdown-файлов.
        // Снятие регистрации (undo) выполняется автоматически при удалении
        // компонента. При обновлении компонента менеджер обновлений выполняет
        // скрипт новой версии заново, поэтому регистрация обновляется вместе с exe.
        // Без прав администратора запись идёт в HKCU\Software\Classes (текущий
        // пользователь), с правами администратора - в HKLM\Software\Classes.
        // ВАЖНО: @TargetDir@ раскрывается с прямыми слэшами - ShellExecute
        // не разбирает такую команду ("отказано в доступе" при открытии
        // файла). Путь к exe нормализуем к обратным слэшам и берём в кавычки.
        var targetDir = installer.value("TargetDir").replace(/\//g, "\\");
        var command = "\"" + targetDir + "\\MarkdownViewer.exe\" \"%1\"";
        var icon = targetDir + "\\markdown-file-icon.ico";
        var progId = "ProgId=RsWorkTools.MarkdownViewer";

        component.addOperation("RegisterFileType", "md", command,
            "Markdown Document", "text/markdown", icon, progId);
        component.addOperation("RegisterFileType", "markdown", command,
            "Markdown Document", "text/markdown", icon, progId);
    }
}
