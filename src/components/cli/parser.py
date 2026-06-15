import argparse


def create_parser():

    parser = argparse.ArgumentParser(prog="qsynq")

    parser.add_argument("--verbose", action="store_true")

    parser.add_argument("--config")

    subparsers = parser.add_subparsers(dest="command")

    sync = subparsers.add_parser("sync")
    sync.add_argument("path")

    watch = subparsers.add_parser("watch")
    watch.add_argument("path")

    add = subparsers.add_parser("add")
    add.add_argument("path")

    remove = subparsers.add_parser("remove")
    remove.add_argument("path")

    subparsers.add_parser("status")
    subparsers.add_parser("config")

    return parser
