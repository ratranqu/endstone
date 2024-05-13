import errno
import functools
import logging
import os
import platform
import sys

import click
from endstone._internal.version import __version__

logging.basicConfig(
    level=logging.INFO,
    format="[%(asctime)s.%(msecs)03d %(levelname)s] [%(name)s] %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S",
)
logger = logging.getLogger(__name__)

__all__ = ["cli"]


def catch_exceptions(func):
    """Decorator to catch and log exceptions."""

    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        try:
            return func(*args, **kwargs)
        except Exception as e:
            logger.exception(e)
            sys.exit(-1)

    return wrapper


@click.command(help="Starts an endstone server.")
@click.option(
    "-s",
    "--server-folder",
    default="bedrock_server/{system}/{version}",
    help="Specify the folder for the bedrock server. Defaults to 'bedrock_server/{system}/{version}'.",
)
@click.option(
    "-y",
    "--no-confirm",
    "--yes",
    default=False,
    is_flag=True,
    show_default=True,
    help="Assume yes as answer to all prompts",
)
@click.option(
    "-r",
    "--remote",
    default="https://raw.githubusercontent.com/EndstoneMC/bedrock-server-data/main/bedrock_server_data.json",
    help="The remote URL to retrieve bedrock server data from.",
)
@click.version_option(__version__)
@catch_exceptions
def cli(server_folder: str, no_confirm: bool, remote: str) -> None:
    """
    Starts an Endstone server.

    Args:
        server_folder (str): The directory where the bedrock server is installed.
        no_confirm (bool): Assume yes as answer to all prompts.
        remote (str): The remote URL to retrieve bedrock server data from.

    Raises:
        NotImplementedError: If the operating system is not supported.
        FileNotFoundError: If the server executable is not found and install is set to False.
    """
    from endstone import __minecraft_version__ as minecraft_version

    system = platform.system()
    if system == "Windows":
        from endstone._internal.bootstrap.windows import WindowsBootstrap

        cls = WindowsBootstrap

    elif system == "Linux":
        from endstone._internal.bootstrap.linux import LinuxBootstrap

        cls = LinuxBootstrap
    else:
        raise NotImplementedError(f"{system} is not supported.")

    bootstrap = cls(server_folder=server_folder, version=minecraft_version, remote=remote)

    if not bootstrap.executable_path.exists():
        if not no_confirm:
            download = click.confirm(
                f"Bedrock Dedicated Server (v{minecraft_version}) "
                f"is not found in {str(bootstrap.executable_path.parent)}. "
                f"Would you like to download it now?",
                default=True,
            )
        else:
            download = True

        if not download:
            raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), str(bootstrap.executable_path))

        bootstrap.install()

    exit_code = bootstrap.run()
    sys.exit(exit_code)
