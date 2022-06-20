class Control:

    def __init__(self, param):
        self.active = False
        self.x = -1
        self.y = -1

        if isinstance(param, str):
            self.path = param
            self.function = None
        else:
            self.path = None
            self.function = param
