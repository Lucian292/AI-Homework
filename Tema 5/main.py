import numpy as np

# Funcții de activare și derivatele acestora
def sigmoid(x):
    return 1 / (1 + np.exp(-x))

def sigmoid_derivative(x):
    return x * (1 - x)

# Funcția de eroare (eroarea medie pătratică)
def mean_squared_error(y_true, y_pred):
    return np.mean((y_true - y_pred) ** 2)

# Citirea datelor din fișier
data = np.genfromtxt('D:\\Universitate\\AI\\Tema 5\\seeds_dataset.txt', delimiter='\t')

# Amestecarea datelor pentru a le face aleatoare
np.random.shuffle(data)

# Împărțirea datelor în set de antrenare și de testare (de exemplu, 80% pentru antrenare și 20% pentru testare)
train_data = data[:int(0.8 * len(data))]
test_data = data[int(0.8 * len(data)):]

# Separarea atributelor (intrări) și claselor (ieșiri)
X_train = train_data[:, :-1]
y_train = train_data[:, -1]
X_test = test_data[:, :-1]
y_test = test_data[:, -1]

# Inițializarea parametrilor rețelei
input_size = X_train.shape[1]
hidden_size = 5
output_size = 3
learning_rate = 0.01
num_epochs = 100

# Inițializarea ponderilor cu valori mici aleatoare
np.random.seed(0)
weights_input_hidden = np.random.randn(input_size, hidden_size)
weights_hidden_output = np.random.randn(hidden_size, output_size)

# Antrenarea rețelei
for epoch in range(num_epochs):

    # Inițializarea ieșirii cu valorile zero
    output = np.zeros((len(X_train), output_size))

    # Propagarea înainte
    hidden_input = np.dot(X_train, weights_input_hidden)
    hidden_output = sigmoid(hidden_input)
    output_input = np.dot(hidden_output, weights_hidden_output)
    output = sigmoid(output_input)

    # Calculează eroarea
    loss = mean_squared_error(y_train, output)

    # Propagarea înapoi
    output_error = y_train - output
    output_delta = output_error * sigmoid_derivative(output)
    hidden_error = output_delta.dot(weights_hidden_output.T)
    hidden_delta = hidden_error * sigmoid_derivative(hidden_output)

    # Actualizarea ponderilor pentru stratul de ieșire
    weights_hidden_output += hidden_output.T.dot(output_delta) * learning_rate

    # Actualizarea ponderilor pentru stratul ascuns
    weights_input_hidden += X_train.T.dot(hidden_delta) * learning_rate

    # Afișarea progresului
    if epoch % 10 == 0:
        print(f'Epoch {epoch}: Loss {loss}')

# Testarea rețelei
hidden_input = np.dot(X_test, weights_input_hidden)
hidden_output = sigmoid(hidden_input)
output_input = np.dot(hidden_output, weights_hidden_output)
output = sigmoid(output_input)

# Calcularea acurateței
correct = 0
for i in range(len(y_test)):
    if np.argmax(output[i]) + 1 == int(y_test[i]):
        correct += 1

accuracy = correct / len(y_test) * 100
print(f'Acuratețe pe datele de testare: {accuracy:.2f}%')
