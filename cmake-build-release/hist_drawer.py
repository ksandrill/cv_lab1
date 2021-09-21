import plotly.graph_objects as go
import numpy as np


def main():
    x = []
    with open('hist_info.txt', 'r') as hf:
        for line in hf:
            x.append(int(line))
    data = [go.Histogram(x=x)]
    fig = go.Figure(data)
    fig.show()


if __name__ == "__main__":
    main()
