class Request():
    def feed_data(self, data: bytes):
        ...

    def get_method(self) -> bytes:
        ...

    def get_path(self) -> bytes:
        ...

    def get_headers(self) -> bytes:
        ...

    def get_body(self) -> bytes:
        ...

    def get_http_version(self) -> bytes:
        ...
