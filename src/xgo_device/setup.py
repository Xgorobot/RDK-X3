from setuptools import setup

package_name = 'xgo_device'

setup(
    name=package_name,
    version='0.0.1',
    packages=[package_name],
    package_data={
        package_name: ['font/msyh.ttc']
    },
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='sunrise',
    maintainer_email='you@example.com',
    description='XGO device info display using SPI LCD',
    license='MIT',
    entry_points={
        'console_scripts': [
            'device_info = xgo_device.device_info:main'
        ],
    },
)
