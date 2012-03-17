import random

CONST_TESTS = """3
5 6 +
65 -99 *
432 123 - 567 +
4 + +
567 876 9 * *
56 abcd
"""

def randomNumber():
    return str(random.randint(-5000, 100000)) + " "

def randomOperator():
    return random.choice(["+", "-", "*"]) + " "

if __name__ == "__main__":
    with open("tests.txt", "w") as outputFile:
        outputFile.write(CONST_TESTS)
        for i in xrange(100):
            test = ""
            test += randomNumber()
            balance = 1
            for j in xrange(i + 10):
                if balance > 1:
                    choice = random.randint(0, 99)
                    if choice < 30:
                        test += randomNumber()
                        balance += 1
                    else:
                        test += randomOperator()
                        balance -= 1
                else:
                    test += randomNumber()
                    balance += 1
            for j in xrange(balance, 1, -1):
                test += randomOperator()
            outputFile.write(test[:-1] + "\n")
