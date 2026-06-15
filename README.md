# qsynq

**qsynq** — учебная система синхронизации файлов для Linux. Проект разделён на C++ daemon `qsynqd`, Python CLI `qsynq` и набор C++ компонентов для файлового слоя, индекса SQLite, diff-логики, сети и менеджера синхронизации.

CLI и daemon общаются локально через Unix domain socket. Формат сообщений — JSON.

## Возможности

- обход файловой системы и формирование snapshot;
- отслеживание изменений через `inotify`;
- хранение состояния файлов в SQLite;
- вычисление diff между снимками состояния;
- TCP transport для передачи файлов;
- daemon-процесс `qsynqd`;
- Python CLI `qsynq`;
- локальный IPC через Unix-socket и JSON.

## Бинарники

После сборки CMake кладёт исполняемые файлы в `build/bin`:

| Файл | Тип | Назначение |
| --- | --- | --- |
| `build/bin/qsynqd` | C++ executable | Фоновый daemon. Слушает Unix-socket и обрабатывает JSON-команды. |
| `build/bin/qsynq` | Python script | CLI-клиент. Отправляет JSON-команды daemon-у. |

Исходный Python CLI находится в `scripts/qsynq` и при сборке копируется в `build/bin/qsynq`.

## Исходники

Основные директории проекта:

```text
.
├── CMakeLists.txt              # главный CMake проекта
├── vcpkg.json                  # зависимости C/C++ уровня
├── scripts/
│   └── qsynq                   # Python CLI, копируется в build/bin
├── src/
│   ├── CMakeLists.txt          # подключает subdirectory компонентов
│   ├── daemon/
│   │   └── qsynqd.cpp          # entrypoint daemon-а
│   ├── cli/
│   │   └── CMakeLists.txt      # CMake target для копирования CLI
│   └── components/
│       ├── common/             # общие типы, например FileEntry
│       ├── file-system-layer/  # FileScanner и FileWatcher
│       ├── index-db/           # SQLite repository и schema initializer
│       ├── logic/              # DiffEngine
│       ├── network/            # TCP transport
│       ├── sync-manager/       # координация scanner/repository/diff
│       └── cli/                # модульные Python-исходники CLI
├── UML/
│   ├── componentDiagram.puml
│   └── useCaseDiagram.puml
└── build/                      # артефакты сборки, игнорируются git
```

Сборка сделана через `add_subdirectory`: у каждого C++ компонента есть свой `CMakeLists.txt`, а главный CMake подключает `src`.

## Зависимости

- Linux;
- C++20 compiler;
- CMake 3.16+;
- Python 3;
- SQLiteCpp;
- SQLite3.

Зависимость `sqlitecpp` указана в `vcpkg.json`. Если пакет установлен через vcpkg, CMake найдёт его через toolchain/prefix. В текущей локальной схеме также поддержан fallback на уже установленный `src/build/vcpkg_installed/x64-linux`.

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

После этого доступны:

```bash
build/bin/qsynqd
build/bin/qsynq
```

## Использование

Запустить daemon:

```bash
build/bin/qsynqd
```

В другом терминале выполнить CLI-команду:

```bash
build/bin/qsynq ping
build/bin/qsynq status
build/bin/qsynq add /home/user/Documents
build/bin/qsynq config
build/bin/qsynq stop
```

CLI может сам стартовать daemon, если `qsynqd` доступен рядом с ним или в `PATH`:

```bash
build/bin/qsynq start
```

## Unix-Socket API

По умолчанию daemon слушает:

```text
/tmp/qsynqd.sock
```

Путь можно переопределить переменной окружения:

```bash
QSYNQ_SOCKET=/tmp/my-qsynqd.sock build/bin/qsynqd
QSYNQ_SOCKET=/tmp/my-qsynqd.sock build/bin/qsynq status
```

Пример JSON-запроса:

```json
{"command": "status"}
```

Пример JSON-ответа:

```json
{
  "ok": true,
  "state": "running",
  "folders": []
}
```

Текущие команды daemon-а:

| Команда | Payload | Назначение |
| --- | --- | --- |
| `ping` | `{"command":"ping"}` | Проверка доступности daemon-а. |
| `status` | `{"command":"status"}` | Состояние daemon-а и список добавленных путей. |
| `add` | `{"command":"add","path":"/path"}` | Добавить путь в текущую сессию daemon-а. |
| `config` | `{"command":"config"}` | Показать активный Unix-socket. |
| `stop` | `{"command":"stop"}` | Остановить daemon. |

## Git Ignore

Артефакты сборки не должны попадать в репозиторий. В `.gitignore` уже добавлены:

- `build/`;
- `src/build/`;
- CMake cache/files;
- generated binaries;
- Python cache.

## Системный сервис

Планируемый production-сценарий — запуск `qsynqd` как systemd-сервиса:

```bash
systemctl start qsynqd
systemctl stop qsynqd
systemctl status qsynqd
```

Unit-файл пока не входит в репозиторий.

## Лицензия

Проект распространяется в образовательных целях в рамках выпускной квалификационной работы.
