from setuptools import setup

package_name = 'xgo_motion'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
         ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='sunrise',
    maintainer_email='sunrise@todo.todo',
    description='Dog show demo for XGO',
    license='MIT',
    include_package_data=True,
    package_data={
        'xgo_motion': ['music/*.mp3', 'expression/*/*.png'],  
    },
    entry_points={
        'console_scripts': [
            'dog_show = xgo_motion.dog_show:main',
        ],
    },
)
