#!/usr/bin/env python3

from cli.commands import CommandDispatcher
from cli.parser import create_parser
from cli.validator import validate_path


def main():

    parser = create_parser()

    args = parser.parse_args()

    payload = vars(args)

    command = payload.get("command")

    if command in {"sync", "watch", "add", "remove"}:
        validate_path(payload["path"])

    dispatcher = CommandDispatcher()

    dispatcher.execute(payload)


if __name__ == "__main__":
    main()
