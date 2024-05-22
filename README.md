# Auto White Balance (AWB) Code

This project is written entirely in C language and provides an Auto White Balance (AWB) solution. Given an input image, the code corrects the white balance and produces an output image with improved color balance.

## Introduction

Auto White Balance (AWB) is a crucial step in image processing to ensure accurate color representation. This C-based AWB code utilizes a Grey World assumption along with advanced filters to eliminate non-gray pixels for final calculation. 

## Features

- Corrects white balance in images automatically.
- Written in C for efficiency and portability.
- Utilizes Grey World assumption for color correction.
- Employs advanced filters to eliminate non-gray pixels for accurate calculation.

## Installation

To use this AWB code, follow these steps:

1. **Clone the repository:**

```bash
$ git clone https://github.com/NotSoIntelligent/sunny-AWB.git
$ cd sunny-AWB
$ make all
$ ./<executable_name> <filename>
