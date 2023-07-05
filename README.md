# Neural Network

A simple neural network implementation in C.

![nn](https://github.com/nxmbit/neural-network/assets/64561905/1754f5a0-1864-4583-92b8-2b2a7146e0b3)

## Usage
By default, there are 3 supported datasets:
* Logic gates
* Color classification
* MNIST (WIP)

To interact with the network, you have to train it or load a pre-trained model.

## Custom datasets
In addition to the provided datasets, you can use your own dataset to train the network. To do this, place the dataset in the csv format in the `datasets/custom` folder. 

Each row in your csv file represents an individual data instance. The data instance consists of multiple 'data_entry' values, followed by the 'expected_value_index', which is the correct index of the output for that instance. The network is designed to have as many neurons in the output layer as there are distinct 'expected_value_index' values across your dataset, and as many neurons in the input layer as there are 'data_entry' values in a data instance.

The dataset has to be formatted in the following way (all entries are separated by a comma):

| data_entry1 | data_entry2 | data_entry3 | .. | expected_value_index |
|-------------|-------------|-------------|----|----------------------|
|    val1     |    val2     |     val3    | .. |    expected_index1   |
|    val1     |    val2     |     val3    | .. |    expected_index2   |
|    ....     |    ....     |     ....    | .. |    expected_index... |

- `data_entry1`, `data_entry2`, `data_entry3`, ..., represent the input values for the neural network. 
- `expected_value_index` indicates the index of the correct output for this set of inputs.
- `val1`, `val2`, `val3`, ... are the actual numerical values for each 'data_entry'.
- `expected_index1`, `expected_index2`, `expected_index...`, are the actual indices for the 'expected_value_index'.

## Testing and Saving Models trained on custom datasets
After training the network on your custom dataset, you can input data from the keyboard for each neuron, and the program will return the index which it determines to be correct according to the trained network.

Moreover, the trained model can be saved to a file for future usage. You can load it back later. The models are saved in the `models/custom` folder.
