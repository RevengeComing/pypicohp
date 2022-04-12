from distutils.core import Extension, setup


ext_modules = [
    Extension(
        'pypicohp.request',
        sources=["pypicohp/request.c", "pypicohp/picohttpparser.c"],
    )
]

setup(
    name="pypicohp",
    version='0.1',
    author='Sepehr Hamzehlouy',
    author_email='s.hamzelooy@gmail.com',
    description='Python binding for picohttpparser.',
    ext_modules=ext_modules,
)
