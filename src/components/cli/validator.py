from pathlib import Path


def validate_path(path: str):
    if not Path(path).exists():
        raise ValueError(f"Path does not exist: {path}")


def validate_interval(interval: int):
    if interval <= 0:
        raise ValueError("Interval must be greater than zero")
