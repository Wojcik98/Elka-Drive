package pl.edu.pw.elka.zpr.elkadrive.controller;

import lombok.RequiredArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.server.ResponseStatusException;
import pl.edu.pw.elka.zpr.elkadrive.domain.UserIdent;
import pl.edu.pw.elka.zpr.elkadrive.repository.UserRepository;

@RestController
@RequestMapping("/register")
@RequiredArgsConstructor
public class RegistrationController {
    private final UserRepository userRepository;
    private final PasswordEncoder passwordEncoder;

    @PostMapping
    public void processRegistration(UserIdent u) {
        userRepository.findByUsername(u.getUsername())
                .ifPresent(userIdent -> {
                    throw new ResponseStatusException(HttpStatus.CONFLICT);
                });

        UserIdent newUserIdent = new UserIdent(
                u.getUsername(),
                passwordEncoder.encode(u.getPassword())
        );
        userRepository.save(newUserIdent);
    }
}
