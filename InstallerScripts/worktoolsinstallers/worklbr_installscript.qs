function Component()
{
}

Component.prototype.createOperations = function()
{
    // Стандартные операции (распаковка данных компонента)
    component.createOperations();

    if (installer.value("os") === "win")
    {
        // Регистрация WorkRes как редактора *.lbr файлов.
        // Каждый файл открывается в отдельном экземпляре приложения -
        // single-instance не используется, просто передаём путь аргументом.
        // Снятие регистрации (undo) выполняется автоматически при удалении
        // компонента; при обновлении скрипт выполняется заново.
        // ВАЖНО: @TargetDir@ раскрывается с прямыми слэшами - ShellExecute
        // не разбирает такую команду ("отказано в доступе" при открытии
        // файла). Путь к exe нормализуем к обратным слэшам и берём в кавычки.
        var targetDir = installer.value("TargetDir").replace(/\//g, "\\");
        var command = "\"" + targetDir + "\\WorkRes.exe\" \"%1\"";
        var icon = targetDir + "\\lbr-file-icon.ico";

        component.addOperation("RegisterFileType", "lbr", command,
            "LBR Resource Library", "application/x-lbr", icon,
            "ProgId=RsWorkTools.LbrFile");
    }
}
