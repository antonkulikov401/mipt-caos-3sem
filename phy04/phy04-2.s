.intel_syntax noprefix
.text
.global mergesort

# mergesort(from, to, *in, *out) - sorts in[from..to)
mergesort:
	push ebp
	mov ebp, esp
	push ebx

	mov eax, [ebp + 8]
    mov ecx, [ebp + 12]
	mov esi, [ebp + 16]
	mov edi, [ebp + 20]
	sub ecx, eax
	sub ecx, 1 # right index becomes inclusive
	imul eax, 4
	add esi, eax
	mov eax, 0

	copy_in_to_out:
	cmp eax, ecx
	jg	start_mergesort
	mov edx, [esi + 4 * eax]
	mov [edi + 4 * eax], edx
	add eax, 1
	jmp copy_in_to_out
	
	start_mergesort:
	push edi
	push ecx
	push 0
	call merge_sort
	pop eax
	pop eax
	pop eax

	pop ebx
	pop ebp
	ret


# merge_sort(from, to, *arr) - sorts arr[from..to]
merge_sort:
	push ebp
	mov ebp, esp

	push eax
	push ecx
	push esi
	
	mov esi, [ebp + 16] # esi = arr
	mov eax, [ebp + 8]  # eax = from
	mov ecx, [ebp + 12]	# ecx = to
	cmp eax, ecx
	je	end_merge_sort # if (from == to) return;

	add eax, ecx
	sar eax, 1 # eax = (from+to)/2;

	push esi
	push eax
	push [ebp + 8]
	call merge_sort # mergesort(from, (from+to)/2, arr);
	pop edx
	pop edx
	pop edx

	inc eax
	push esi
	push [ebp + 12]
	push eax
	call merge_sort # mergesort((from+to)/2+1, to, arr);
	pop edx
	pop edx
	pop edx

	dec eax
	push esi
	push [ebp + 12]
	push eax
	push [ebp + 8]
	call merge		# merge(from, (from+to)/2, to, arr);
	pop edx
	pop edx
	pop edx
	pop edx

	end_merge_sort:

	pop esi
	pop ecx
	pop eax

	pop ebp
	ret


# merge(i, j, k, *arr) - merges arr[i..j] and arr[j+1..k]
merge:
	push ebp
	mov ebp, esp

	mov esi, [ebp + 20] # esi = arr
	mov eax, [ebp + 8]  # eax = left pointer
	mov ecx, [ebp + 12]
	inc ecx				# ecx = right pointer
	mov edx, [ebp + 16]	# edx - index for final merge


	push_in_stack: # push elements in stack in sorted order 
	cmp eax, [ebp + 12]
	jg	push_remaining_left_part # if (left pointer > j)
	cmp ecx, [ebp + 16]
	jg	push_remaining_left_part # if (right pointer > k)

	mov edi, [esi + 4 * eax]
	cmp edi, [esi + 4 * ecx]
	jle push_left_part 	# if (arr[left] <= arr[right]) push_left_part;
	jmp push_right_part	# else push_right_part;
	push_left_part:
		push [esi + 4 * eax]
		inc eax
		jmp push_in_stack
	push_right_part:
		push [esi + 4 * ecx]
		inc ecx
		jmp push_in_stack


	push_remaining_left_part:
	cmp eax, [ebp + 12]
	jg	push_remaining_right_part
	push [esi + 4 * eax]
	inc eax
	jmp push_remaining_left_part

	push_remaining_right_part:
	cmp ecx, [ebp + 16]
	jg	pop_in_array
	push [esi + 4 * ecx]
	inc ecx
	jmp push_remaining_right_part


	pop_in_array: # pop sorted elements from stack in array in reverse order
	cmp edx, [ebp + 8]
	jl	end_merge
	pop edi
	mov [esi + 4 * edx], edi
	dec edx
	jmp pop_in_array

	end_merge:
	pop ebp
	ret

