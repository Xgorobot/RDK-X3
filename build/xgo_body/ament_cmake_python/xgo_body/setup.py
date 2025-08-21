from setuptools import find_packages
from setuptools import setup

setup(
    name='xgo_body',
    version='0.0.0',
    packages=find_packages(
        include=('xgo_body', 'xgo_body.*')),
)
