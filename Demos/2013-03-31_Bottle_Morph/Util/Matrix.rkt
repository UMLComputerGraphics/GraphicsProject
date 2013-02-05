#lang racket
(require htdp/matrix)
(require rackunit)

(define transpose
  (lambda (mat1)
    
    (mat4
     (matrix-ref mat1 0 0)
     (matrix-ref mat1 1 0)
     (matrix-ref mat1 2 0)
     (matrix-ref mat1 3 0)
     (matrix-ref mat1 0 1)
     (matrix-ref mat1 1 1)
     (matrix-ref mat1 2 1)
     (matrix-ref mat1 3 1)
     (matrix-ref mat1 0 2)
     (matrix-ref mat1 1 2)
     (matrix-ref mat1 2 2)
     (matrix-ref mat1 3 2)
     (matrix-ref mat1 0 3)
     (matrix-ref mat1 1 3)
     (matrix-ref mat1 2 3)
     (matrix-ref mat1 3 3)
     )))

(define float_trunc
  (lambda (num precision)
    (let ([scale (expt 10 precision)])
      (/ (round (* num scale)) scale))))

(define float_trunc_4
  (lambda (num)
    (float_trunc num 4)))

(define mat4
  (lambda cell-list
    (make-matrix 4 4 
                 (map
                  float_trunc_4
                  cell-list))))

(define vec4
  (lambda cell-list
    (make-matrix 4 1
                 (map
                  float_trunc_4
                  cell-list))))

(define 4vec
  (lambda cell-list
    (make-matrix 1 4
                 (map
                  float_trunc_4
                  cell-list))))

(define RotateY
  (lambda (angle)
    (let ([radAngle (degrees->radians angle)])
      (mat4
       (cos radAngle) 0 (sin radAngle) 0
       0 1 0 0
       (- (sin radAngle)) 0 (cos radAngle) 0
       0 0 0 1))))

(define RotateX
  (lambda (angle)
    (let ([radAngle (degrees->radians angle)])
      (mat4
       1 0              0                 0
       0 (cos radAngle) (-(sin radAngle)) 0
       0 (sin radAngle) (cos radAngle)    0
       0 0              0                 1))))

(define RotateZ
  (lambda (angle)
    (let ([radAngle (degrees->radians angle)])
      (mat4
       (cos radAngle) (-(sin radAngle)) 0 0
       (sin radAngle) (cos radAngle) 0 0
       0 0 1 0
       0 0 0 1))))

; Create an Identity matrix.
(define I
  (lambda (n)
    (make-matrix n n 
    (build-list
     (expt n 2)
     (lambda (i)
       (if (= (remainder i (+ n 1)) 0)
           1
           0))))))
           
; Obtain the value of a single cell of the matrix produced by (mat1*mat2).
; Indexed by [row][col].
(define _V*
  (lambda (mat1 mat2 1row 2col)    
    (letrec ([_C* 
              (lambda (n sum)
                (if (= n 0) sum
                    (+ 
                     (* (matrix-ref mat1 1row (- n 1)) (matrix-ref mat2 (- n 1) 2col))
                     (_C* (- n 1) sum))))])
      (_C* (matrix-cols mat1) 0))))

; Produce the 'r'th row of the matrix produced by (mat1*mat2).
(define _R*
  (lambda (mat1 mat2 r)
    (build-list
     (matrix-cols mat2)
     (lambda (n)
       (_V* mat1 mat2 r n)))))

; Multiply two matrices.
(define _M*
  (lambda (mat1 mat2)
    (if (not (= (matrix-cols mat1) (matrix-rows mat2)))
        (raise "Error: #Cols on Mat1 must match #Rows on Mat2.")
        (make-matrix (matrix-rows mat1) (matrix-cols mat2)
                     (foldr
                      append
                      empty
                      (build-list
                       (matrix-rows mat1)
                       (lambda (r)
                         (_R* mat1 mat2 r))))))))

; Multiply any number of matrices.
(define M*
  (lambda matlist
    (if (empty? matlist) empty
        (let ([ID (I (matrix-cols (first matlist)))])
          (foldl
           ;; Foldl will call your function as (func head init)
           ;; I.e, (M1 * M0)
           ;; Which is a reversal of the correct multiplication order. So we reverse it all stealthlike.
           (lambda (x y) (_M* y x))
           ID
           matlist)))))

(define A
  (mat4
   1 2 3 4
   5 6 7 8
   9 1 2 3
   4 5 6 7))

(define B
  (mat4
   9 8 7 6
   5 4 3 2
   1 9 8 7
   6 5 4 3))

(define AB
  (M* A B))

(define BA
  (M* B A))

(define ATBT
  (M* (transpose A) (transpose B)))

(define BTAT
  (M* (transpose B) (transpose A)))