import logging

LOGGER = logging.getLogger("MOOGLI - Integrated Visualization Environment")
LOGGER.setLevel(logging.DEBUG)
formatter = logging.Formatter("[%(asctime)s]-[%(levelname)s]-[%(filename)s:%(funcName)s:%(lineno)s]- %(message)s")
console = logging.StreamHandler()
console.setFormatter(formatter)
console.setLevel(logging.DEBUG)
LOGGER.addHandler(console)

DEBUG = LOGGER.debug
INFO = LOGGER.info
WARN = LOGGER.warn
ERROR = LOGGER.error
CRITICAL = LOGGER.critical
EXCEPTION = LOGGER.exception
