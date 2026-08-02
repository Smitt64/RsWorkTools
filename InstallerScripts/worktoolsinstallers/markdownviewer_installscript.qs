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
        var command = "@TargetDir@\\MarkdownViewer.exe \"%1\"";
        var icon = "@TargetDir@\\markdown-file-icon.ico";
        var progId = "ProgId=RsWorkTools.MarkdownViewer";

        component.addOperation("RegisterFileType", "md", command,
            "Markdown Document", "text/markdown", icon, progId);
        component.addOperation("RegisterFileType", "markdown", command,
            "Markdown Document", "text/markdown", icon, progId);
    }
}
